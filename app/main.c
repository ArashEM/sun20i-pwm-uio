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
        .period = {.entire = 3500 - 1, .act = 1500 },
        .pre = 9,
        .state = ACT_HIGH,
        .en = true,
    };
    set_pwm_config(p, 4, &pwm);

    // Capture channel
    struct cap_config cap = {
        .clk = {.src = APB0, .div = DIV_8},
        .pre = 0,
        .falling = true,
        .rising = true,
    };
    set_cap_config(p, 2, &cap);

    struct cap_result_raw raw;
    cap_blocking(p, 2, &raw);
    cap_en(p, 2, false, false);

    printf("on: %d, off: %d\n", raw.on_cycles, raw.off_cycles);

    munmap(p, 4096);
    close(fd);
    return 0;
}