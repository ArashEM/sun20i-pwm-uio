#ifndef PWM_H
#define PWM_H
/**
 * @brief Low level functions to read/write to PWM registers
 * 
 */
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

#include "pwm_types.h"
#include "registers.h"
#include "rw.h"

/**
 * @brief Pass / Mask PWM clock
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param pass true: Pass the clock, false: gate the clock
 * @return int32_t 0 on success
 */
inline int32_t clk_gate(void *base, uint8_t ch, bool pass)
{
    if(check_ch(ch))
        return -EINVAL;

    rmwb(base + PCGR_OFFSET, PWMx_CLK_GATING(ch), pass);

    return 0;
}

/**
 * @brief Bypass PWM clock to it's output pin
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param bypass true: Bypass the clock, false: Not bypass
 * @return int32_t 0 on success
 */
inline int32_t clk_bypass(void *base, uint8_t ch, bool bypass)
{
    if(check_ch(ch))
        return -EINVAL;

    rmwb(base + PCGR_OFFSET, PWMx_CLK_BYPASS(ch), bypass);

    return 0;
}

/**
 * @brief Enable PWM channel
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param en true: Enable PWM, false: Disable PWM
 * @return int32_t 0 on success 
 */
inline int32_t pwm_en(void *base, uint8_t ch, bool en)
{
    if(check_ch(ch))
        return -EINVAL;

    rmwb(base + PER_OFFSET, PWMx_EN(ch), en);

    return 0;
}

/**
 * @brief Configure PWM clock 
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7] 
 * @param clk Clock configuration (Source and Divider)
 * @return int32_t 0 on success
 * @note It's shared between (c) and (ch + 1). Please check reference manual
 */
inline int32_t clk_config(void *base, uint8_t ch, struct pwm_clk clk)
{
    if(check_ch(ch))
        return -EINVAL;

    if(check_clk(clk))
        return -EINVAL;

    writel(base + PCCRxy_OFFSET(ch), PCCRxy_VALUE(clk.src, clk.div));
    
    return 0;
}

/**
 * @brief Set PWM pre scaler
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7] 
 * @param pre Clock pre scaler
 * @return int32_t 0 on success
 */
inline int32_t pwm_prescaler(void *base, uint8_t ch, uint8_t pre)
{
    if(check_ch(ch))
        return -EINVAL;

    uint32_t reg = readl(base + PWM_REG_OFFSET(PCR_OFFSET, ch));
    SET_PWM_PRESCALE(reg, pre);
    writel(base + PWM_REG_OFFSET(PCR_OFFSET, ch), reg);

    return 0;
}

/**
 * @brief Set PWM period (entire, act) value
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param period Entire and Active cycles
 * @return int32_t 0 on success
 */
inline int32_t pwm_period(void *base, uint8_t ch, struct pwm_period period)
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

/**
 * @brief Configure PWM active state (low/high)
 * 
 * @param p 
 * @param ch 
 * @param state 
 * @return int32_t 
 */
inline int32_t pwm_act_state(void *p, uint8_t ch, enum act_state state)
{
    if(check_ch(ch))
        return -EINVAL;

    void *addr = p + PWM_REG_OFFSET(PCR_OFFSET, ch);
    rmwb(addr, PWM_ACT_STA, to_act_state(state));

    return 0;
}


#endif // PWM_H