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
int32_t check_period(struct pwm_period period);

/**
 * @brief True or False based on active state
 * 
 * @param state 
 * @return true : Active High
 * @return false : Active Low
 */
bool to_act_state(enum act_state state);

/**
 * @brief Bypass PWM clock to it's output pin
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param bypass true: Bypass the clock, false: Not bypass
 * @return int32_t 0 on success
 */
int32_t clk_bypass(void *base, uint8_t ch, bool bypass);

/**
 * @brief Check if PWM channel is enabled
 * 
 * @param base 
 * @param ch 
 * @param en 
 * @return int32_t 
 */
int32_t is_pwm_en(void *base, uint8_t ch, bool *en);

/**
 * @brief Enable PWM channel
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param en true: Enable PWM, false: Disable PWM
 * @return int32_t 0 on success 
 * @note In case of disabling channel, this method block until current cycle finish
 */
int32_t pwm_en(void *base, uint8_t ch, bool en);

/**
 * @brief Set PWM period (entire, act) value
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param period Entire and Active cycles
 * @return int32_t 0 on success
 */
int32_t set_period(void *base, uint8_t ch, struct pwm_period period);

/**
 * @brief Report current Entire/Active cycles of PWM channel
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param period Entire and Active cycles
 * @return int32_t 0 on success
 */
int32_t get_period(void *base, uint8_t ch, struct pwm_period *period);

/**
 * @brief Configure PWM active state (low/high)
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param state Active high / low state
 * @return int32_t 0 on success
 */
int32_t set_act_state(void *p, uint8_t ch, enum act_state state);

#endif // PWM_H