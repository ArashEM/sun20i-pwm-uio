#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

#include "pwm.h"

int main() {
    int fd = open("/dev/uio0", O_RDWR);
    void *p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // volatile u_int32_t *base = p + 0xc00;
    p += 0xc00;

    //base[0x40 / 4] = 0x04; // PWM2 gating
    //base[0x24 / 4] = 0x04; // 24Mhz / 256
    //base[0x40 / 4] = 0x40004; // PWM2 bypass
    //base[0x80 / 4] = 0x04; // PWM2 enable

    //writel(p + PCGR_OFFSET, PWMx_CLK_GATING(3));
    //writel(p + PCCR23_OFFSET, 0x06);
    //writel(p + PCGR_OFFSET, PWMx_CLK_GATING(3) | PWMx_CLK_BYPASS(3));
    //writel(p + PER_OFFSET, PWMx_EN(3));

    clk_gate(p, 2, true);
    struct pwm_clk clk = {.src = HOSC, .div = DIV_32 };
    clk_config(p, 2, clk);
    clk_bypass(p, 2, true);
    pwm_en(p, 2, true);

    munmap(p, 4096);
    close(fd);
    return 0;
}