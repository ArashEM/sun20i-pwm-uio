/**
 * @file pwm.c
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief T113 PWM mode API
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
#include "pwm.h"

 int32_t check_period(struct pwm_period period)
{
    return (period.act <= period.entire) ? 0 : -EINVAL;
}

 bool to_act_state(enum act_state state)
{
    return (state == ACT_HIGH) ? true : false;
}

 int32_t clk_bypass(void *base, uint8_t ch, bool bypass)
{
    if(check_ch(ch))
        return -EINVAL;

    rmwb(base + PCGR_OFFSET, PWMx_CLK_BYPASS(ch), bypass);

    return 0;
}

 int32_t is_pwm_en(void *base, uint8_t ch, bool *en)
{
    if(check_ch(ch))
        return -EINVAL;
    
    if(!en)
        return -EFAULT;

    uint32_t reg = readl(base + PER_OFFSET);
    *en = IS_SET(reg, PWMx_EN(ch));

    return 0;
}

 int32_t pwm_en(void *base, uint8_t ch, bool en)
{
    if(check_ch(ch))
        return -EINVAL;
    /* when disabling, wait for current cycle to end */
    if(!en) {
        bool cur;
        is_pwm_en(base, ch, &cur);
        uint32_t reg = readl(base + PWM_REG_OFFSET(PPCNTR_OFFSET, ch));
        while(cur & PPCNTR(reg));
    }

    rmwb(base + PER_OFFSET, PWMx_EN(ch), en);

    return 0;
}

 int32_t set_period(void *base, uint8_t ch, struct pwm_period period)
{
    if(check_ch(ch))
        return -EINVAL;
    
    if(check_period(period))
        return -EINVAL;

    uint32_t p = 0;
    SET_PWM_PERIOD(p, period.entire, period.act);
    writel(base + PWM_REG_OFFSET(PPR_OFFSET, ch), p);

    return 0;
}

 int32_t get_period(void *base, uint8_t ch, struct pwm_period *period)
{
    if(check_ch(ch))
        return -EINVAL;

    if(!period)
        return -EFAULT;

    uint32_t reg = readl(base + PWM_REG_OFFSET(PPR_OFFSET, ch));

    period->act = GET_PWM_ACT(reg);
    period->entire = GET_PWM_ENTIRE(reg);

    return 0;
}

 int32_t set_act_state(void *p, uint8_t ch, enum act_state state)
{
    if(check_ch(ch))
        return -EINVAL;

    void *addr = p + PWM_REG_OFFSET(PCR_OFFSET, ch);
    rmwb(addr, PWM_ACT_STA, to_act_state(state));

    return 0;
}