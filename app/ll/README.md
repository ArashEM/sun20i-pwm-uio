# Introduction
This directory includes _Low Level_ utilities for accessing `T113-S3` PWM peripheral.  
Normally you don't need to use this APIs. 

NOTE: This APIs may have conflict with each other (e.g `pwm` and `capture`). Use them carefully.

# Files 
1. `capture.h`: Capture mode configuration. This APIs can conflict with PWM APIs
1. `pwm`: PWM mode configuration. This APIs can conflict with Capture APIs
1. `bitops.h`: Bit operations helper macros
1. `clk.h`: Clock configuration APIs
1. `register.h`: Register index and masks
1. `rw.h`: API to read/write from/to memory location
1. `soc.h`: Some `T133-S3` specific definitions

# Example
PWM mode
```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

#include "clk.h"
#include "pwm.h"

int main() 
{
    // Map PWM UIO device node
    int fd = open("/dev/uio0", O_RDWR);
    void *p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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
}
```