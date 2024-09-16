#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

#include "clk.h"
#include "pwm.h"
#include "capture.h"

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
    struct pwm_period prd = {.entire = 1000 - 1, .act = 151};
    set_period(p, ch, prd);
    set_prescaler(p, ch, 49);
    set_act_state(p, ch, ACT_HIGH);
    pwm_en(p, ch, true);

    // Ch 3 as Capture rising edge only
    ch = 3;
    clk_gate(p, ch, true);
    clk.div = DIV_2;
    clk_config(p, ch, clk);
    set_prescaler(p, ch, 49);
    cap_en(p, ch, true, false);

    size_t i = 0;
    while( i < 10 ) {
        
        sleep(1);
    }


    munmap(p, 4096);
    close(fd);
    return 0;
}