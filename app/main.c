#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

#include "ll/clk.h"
#include "ll/pwm.h"
#include "ll/capture.h"

int main() {
    int fd = open("/dev/uio0", O_RDWR);
    void *p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // volatile u_int32_t *base = p + 0xc00;
    p += 0xc00;


    // Ch 2 as PWM generator 
    uint8_t ch = 2;
    clk_gate(p, ch, true);
    struct pwm_clk clk = {.src = HOSC, .div = DIV_32 };
    clk_config(p, ch, clk);
    struct pwm_period prd = {.entire = 1200 - 1, .act = 730};
    set_period(p, ch, prd);
    set_prescaler(p, ch, 49);
    set_act_state(p, ch, ACT_HIGH);
    pwm_en(p, ch, true);

    // Ch 3 as Capture rising edge only
    ch = 3;
    clk_gate(p, ch, true);
    set_prescaler(p, ch, 4);
    cap_en(p, ch, true, true);
    // clear_cap_irq(p, ch, true, true);

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
        usleep(1000*1000);
    }
    cap_en(p, ch, false, false);


    munmap(p, 4096);
    close(fd);
    return 0;
}