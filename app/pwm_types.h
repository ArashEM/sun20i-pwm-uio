#ifndef PWM_TYPES_H
#define PWM_TYPES_H
/**
 * @brief Type definitions for T113 PWM register access
 * 
 */
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

// PWM Clock Configuration

/**
 * @brief PWM clock source
 * 
 */
enum clk_src {
    HOSC = 0,       // 24Mhz crystal oscillator
    APB0 = 1        // 100Mhz APBO bus clock
};

/**
 * @brief PWM clock divider 
 * @note It's shared between (ch) and (ch + 1)
 */
enum clk_div {
    DIV_1 =     0x00,
    DIV_2 =     0x01,
    DIV_4 =     0x02,
    DIV_8 =     0x03,
    DIV_16 =    0x04,
    DIV_32 =    0x05,
    DIV_64 =    0x06,
    DIV_128 =   0x07,
    DIV_256 =   0x08
};

/**
 * @brief Active high/low state of out pulse
 * 
 */
enum act_state {
    ACT_LOW =   0x00,
    ACT_HIGH =  0x01
};

/**
 * @brief Input clock configuration
 * 
 */
struct pwm_clk {
    enum clk_src src;
    enum clk_div div;
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
 * @brief Check PWM channel range
 * 
 * @param ch 
 * @return int32_t 0 on success
 */
inline int32_t check_ch(uint8_t ch) 
{
    return (ch < 8) ? 0 : -EINVAL;
}

/**
 * @brief Check source and divider range
 * 
 * @param clk 
 * @return int32_t 0 on success
 */
inline int32_t check_clk(struct pwm_clk clk)
{
    if(clk.src > 1 || clk.div > 8)
        return -EINVAL;
    else
        return 0;
}

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

#endif // PWM_TYPES_H