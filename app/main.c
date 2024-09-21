#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

#include "clk.h"
#include "pwm.h"
#include "capture.h"
#include "config.h"

int main() {
    int fd = open("/dev/uio0", O_RDWR);
    void *p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // volatile u_int32_t *base = p + 0xc00;
    p += 0xc00;


    struct pwm_config config = {
        .clk = {.src = APB0, .div = DIV_8},
        .period = {.entire = 12500 - 1, .act = 0},
        .pre = 0,
        .state = ACT_HIGH,
        .en = true,
    };
    set_pwm_config(p, 4, &config);

#if 0
    // Ch 3 as PWM generator 
    uint8_t ch = 3;
    clk_gate(p, ch, true);
    struct pwm_clk clk = {.src = HOSC, .div = DIV_16 };
    clk_config(p, ch, clk);
    struct pwm_period prd = {.entire = 1500 - 1, .act = 1200};
    set_period(p, ch, prd);
    set_prescaler(p, ch, 0);
    set_act_state(p, ch, ACT_HIGH);
    pwm_en(p, ch, true);

    // Ch 5 as Capture
    ch = 5;
    clk_gate(p, ch, true);
    clk.div = DIV_8;
    clk_config(p, ch, clk);
    set_prescaler(p, ch, 2);
    cap_en(p, ch, true, true);
    clear_cap_irq(p, ch, true, true);

    bool loop = true;
    while( loop ) {
        // uint32_t tmp = readl(p + PWM_REG_OFFSET(CCR_OFFSET, ch));
        // printf("ccr: 0x%x\n", tmp);
        // uint16_t lock;
        // cap_rising_lock(p, ch, &lock);
        // printf("crlr: %d\n", lock);
        // cap_falling_lock(p, ch, &lock);
        // printf("cflr: %d\n", lock);
        // clear_cap_irq(p, ch, true, true);
        // sleep(1);
        bool cflf;
        cap_cflf(p, ch, &cflf);

        bool crlf;
        cap_crlf(p, ch, &crlf);

        if(crlf & cflf) {
            uint16_t lock;
            cap_rising_lock(p, ch, &lock);
            printf("crlr: %d\n", lock);
            cap_falling_lock(p, ch, &lock);
            printf("cflr: %d\n", lock);
            clear_cap_irq(p, ch, true, true);
            loop = false;
        }
        usleep(1000*100);
    }
    cap_en(p, ch, false, false);
#endif

    munmap(p, 4096);
    close(fd);
    return 0;
}