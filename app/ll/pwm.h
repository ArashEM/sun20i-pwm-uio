#ifndef PWM_H
#define PWM_H
/**
 * @file pwm.h
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief T113 PWM mode API
 * @version 0.1
 * @date 2024-09-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

#include "soc.h"
#include "rw.h"
#include "registers.h"

/**
 * @brief Active high/low state of out pulse
 * 
 */
enum act_state {
    ACT_LOW =   0x00,
    ACT_HIGH =  0x01
};

/**
 * @brief PWM entire and active period 
 * 
 */
struct pwm_period {
    uint16_t entire;
    uint16_t act;
};

/**
 * @brief Active duration can not be greater than Entire
 * 
 * @param period 
 * @return int32_t 0 on success
 */
inline int32_t check_period(struct pwm_period period)
{
    return (period.act <= period.entire) ? 0 : -EINVAL;
}

/**
 * @brief True or False based on active state
 * 
 * @param state 
 * @return true : Active High
 * @return false : Active Low
 */
inline bool to_act_state(enum act_state state)
{
    return (state == ACT_HIGH) ? true : false;
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
 * @brief Check if PWM channel is enabled
 * 
 * @param base 
 * @param ch 
 * @param en 
 * @return int32_t 
 */
inline int32_t is_pwm_en(void *base, uint8_t ch, bool *en)
{
    if(check_ch(ch))
        return -EINVAL;
    
    if(!en)
        return -EFAULT;

    uint32_t reg = readl(base + PER_OFFSET);
    *en = IS_SET(reg, PWMx_EN(ch));

    return 0;
}

/**
 * @brief Enable PWM channel
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param en true: Enable PWM, false: Disable PWM
 * @return int32_t 0 on success 
 * @note In case of disabling channel, this method block until current cycle finish
 */
inline int32_t pwm_en(void *base, uint8_t ch, bool en)
{
    if(check_ch(ch))
        return -EINVAL;
    /* when disabling, wait for current cycle to end */
    if(!en) {
        bool cur;
        is_pwm_en(base, ch, &cur);
        while(cur & readl(base + PWM_REG_OFFSET(PPCNTR_OFFSET, ch)));
    }

    rmwb(base + PER_OFFSET, PWMx_EN(ch), en);

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
inline int32_t set_period(void *base, uint8_t ch, struct pwm_period period)
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
 * @brief Report current Entire/Active cycles of PWM channel
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param period Entire and Active cycles
 * @return int32_t 0 on success
 */
inline int32_t get_period(void *base, uint8_t ch, struct pwm_period *period)
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

/**
 * @brief Configure PWM active state (low/high)
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param state Active high / low state
 * @return int32_t 0 on success
 */
inline int32_t set_act_state(void *p, uint8_t ch, enum act_state state)
{
    if(check_ch(ch))
        return -EINVAL;

    void *addr = p + PWM_REG_OFFSET(PCR_OFFSET, ch);
    rmwb(addr, PWM_ACT_STA, to_act_state(state));

    return 0;
}

#endif // PWM_H