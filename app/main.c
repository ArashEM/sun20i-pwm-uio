#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

#include "config.h"

int main() {
    // UIO memory mapped device
    int fd = open("/dev/uio0", O_RDWR);
    void *p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    p += 0xc00;

    // PWM channel
    struct pwm_config pwm = {
        .clk = {.src = APB0, .div = DIV_8},
        .period = {.entire = 1250 - 1, .act = 500 },
        .pre = 9,
        .state = ACT_HIGH,
        .en = true,
    };
    set_pwm_config(p, 2, &pwm);


    // Capture channel
    struct cap_config cap = {
        .clk = {.src = HOSC, .div = DIV_1},
        .pre = 23,
        .falling = true,
        .rising = true,
    };
    set_cap_config(p, 4, &cap);

    for(size_t i = 0; i < 5; ++i) {
        struct cap_result_raw raw;
        struct cap_result res;
        cap_blocking(p, 4, &raw);
        result_to_ns(p, 4, &raw, &res);
        printf("on: %d, off: %d\n", raw.on_cycles, raw.off_cycles);
        printf("on: %lld nS, off: %lld nS\n", res.on_ns, res.off_ns);
        sleep(1);
    }

    cap_en(p, 2, false, false);
    cap_en(p, 5, false, false);
    pwm_en(p, 3, false);
    pwm_en(p, 4, false);

    munmap(p, 4096);
    close(fd);
    return 0;
}