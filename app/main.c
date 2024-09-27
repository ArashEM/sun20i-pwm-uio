#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

#include "config.h"
#include "pid.h"

int main() {
    // UIO memory mapped device
    int fd = open("/dev/uio0", O_RDWR);
    void *p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    p += 0xc00;

    // PWM channel
    struct pwm_config pwm = {
        .clk = {.src = APB0, .div = DIV_1},
        .period = {.entire = 1000 - 1, .act = 600 },
        .pre = 99,
        .state = ACT_HIGH,
        .en = true,
    };
    set_pwm_config(p, 2, &pwm);
    set_pwm_config(p, 3, &pwm);
    uint64_t freq;
    get_pwm_freq(p, 2, &freq);
    printf("freq: %lld Hz\n", freq);


    // Capture channel
    struct cap_config cap = {
        .clk = {.src = HOSC, .div = DIV_8},
        .pre = 2,
        .falling = true,
        .rising = true,
    };
    set_cap_config(p, 4, &cap);
    set_cap_config(p, 5, &cap);

    uint64_t max_cap;
    cap_max_duration(p, 4, &max_cap);
    printf("max: %lld uS\n", max_cap/1000UL);

    struct pid_params pid_l = {
        .max = 100,
        .min = -10,
        .dt = 0.1,
        .kp = 0.1,
        .kd = 0.01,
        .ki = 0.5
    };

    struct pid_params pid_r = {
        .max = 100,
        .min = -10,
        .dt = 0.1,
        .kp = 0.1,
        .kd = 0.01,
        .ki = 0.5
    };

    float duty_cycle_r = 60;
    float duty_cycle_l = 60;
    const float rpm_target = 70;
    for(size_t i = 0; i < 30; ++i) {
        struct cap_result_raw raw;
        struct cap_result res;
        cap_blocking(p, 5, &raw);
        result_to_ns(p, 5, &raw, &res);
        uint64_t tach = (res.on_ns + res.off_ns) / 1000000UL;
        uint16_t rpm = to_rpm(res.on_ns + res.off_ns, 20);

        double inc = calculate(&pid_r, rpm_target, rpm);
        duty_cycle_r += inc;
        set_pwm_duty(p, 3, duty_cycle_r);

        printf("ch[r]: %lld mS, rpm: %d, inc: %f, duty:%f\n", 
                        tach, rpm, inc, duty_cycle_r);

        // --------------------
        cap_blocking(p, 4, &raw);
        result_to_ns(p, 4, &raw, &res);
        tach = (res.on_ns + res.off_ns) / 1000000UL;
        rpm = to_rpm(res.on_ns + res.off_ns, 20);

        inc = calculate(&pid_l, rpm_target, rpm);
        duty_cycle_l += inc;
        set_pwm_duty(p, 2, duty_cycle_l);

        printf("ch[l]: %lld mS, rpm: %d, inc: %f, duty:%f\n", 
                        tach, rpm, inc, duty_cycle_l);

        usleep(300 * 1000);
    }

    cap_en(p, 4, false, false);
    cap_en(p, 5, false, false);

    set_pwm_duty(p, 2, 0);
    set_pwm_duty(p, 3, 0);

    munmap(p, 4096);
    close(fd);
    return 0;
}