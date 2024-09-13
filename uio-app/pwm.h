#ifndef PWM_H
#define PWM_H
/**
 * @brief Low level functions to read/write to PWM registers
 * 
 */
#include <errno.h>
#include <stdbool.h>

#include "pwm_types.h"
#include "registers.h"
#include "rw.h"

inline int32_t check_ch(uint8_t ch) 
{
    return (ch < 8) ? 0 : -EINVAL;
}

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
 * @param base 
 * @param ch 
 * @param config 
 * @return int32_t
 * @note It's shared between (c) and (ch + 1). Please check reference manual
 */
inline int32_t clk_config(void *base, uint8_t ch, struct pwm_clk config)
{
    if(check_ch(ch))
        return -EINVAL;

    uint32_t tmp = (config.src << PWM_CLK_SRC_SEL) | config.div;
    writel(base + PCCRxy_OFFSET(ch), tmp);
    
    return 0;
}


#endif // PWM_H