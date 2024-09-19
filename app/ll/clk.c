/**
 * @file clk.c
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief T113-S3 PWM/Capture clock configuration
 * @version 0.1
 * @date 2024-09-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "soc.h"
#include "rw.h"
#include "registers.h"
#include "bitops.h"
#include "clk.h"


int32_t check_clk(struct pwm_clk clk)
{
    if(clk.src > 1 || clk.div > 8)
        return -EINVAL;
    else
        return 0;
}

int32_t clk_gate(void *base, uint8_t ch, bool pass)
{
    if(check_ch(ch))
        return -EINVAL;

    rmwb(base + PCGR_OFFSET, PWMx_CLK_GATING(ch), pass);

    return 0;
}

int32_t clk_config(void *base, uint8_t ch, struct pwm_clk clk)
{
    if(check_ch(ch))
        return -EINVAL;

    if(check_clk(clk))
        return -EINVAL;

    writel(base + PCCRxy_OFFSET(ch), PCCRxy_VALUE(clk.src, clk.div));
    
    return 0;
}

int32_t set_prescaler(void *base, uint8_t ch, uint8_t pre)
{
    if(check_ch(ch))
        return -EINVAL;

    uint32_t reg = readl(base + PWM_REG_OFFSET(PCR_OFFSET, ch));
    SET_PWM_PRESCALE(reg, pre);
    writel(base + PWM_REG_OFFSET(PCR_OFFSET, ch), reg);

    return 0;
}