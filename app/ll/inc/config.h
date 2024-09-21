#ifndef CONFIG_H
#define CONFIG_H
/**
 * @file config.h
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief PWM / Capture configurations (high level API)
 * @version 0.1
 * @date 2024-09-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stdint.h>
#include "clk.h"
#include "pwm.h"

/**
 * @brief Overall configuration of PWM channel
 *
 */
struct pwm_config
{
    struct pwm_clk clk;         // clock source and divider
    uint8_t pre;                // pre-scaler
    struct pwm_period period;   // period and duty cycle
    enum act_state state;       // active high/low
    bool en;                    // channel enable / disable
};

/**
 * @brief Fill config feild in order to generate PWM pulse
 *        based on required period and duty cycle
 *
 * @param period_ns PWM period in nS
 * @param duty_cycle PWM duty cycle in percent (0 to 100)
 * @param config Pointer to PWM configuration to be filled
 * @return 0 on success
 */
int32_t pwm_calc(uint64_t period_ns, uint8_t duty_cycle, struct pwm_config *config);

/**
 * @brief Read current configuration of PWM channel
 *
 * @param p Pointer to PWM base address
 * @param ch PWM channel (0 to 7)
 * @param config Pointer to PWM configuration to be filled witch current configuration
 * @return int32_t 0 on success
 */
int32_t get_pwm_config(void *p, uint8_t ch, struct pwm_config *config);

/**
 * @brief Apply configuration in PWM registers
 * 
 * @param p Pointer to PWM base address
 * @param ch PWM channel (0 to 7)
 * @param config Pointer to configuration
 * @return int32_t 0 on success
 */
int32_t set_pwm_config(void *p, uint8_t ch, const struct pwm_config *config);

/**
 * @brief Set the pwm duty cycle (in percentage)
 * 
 * @param p Pointer to PWM base address
 * @param ch PWM channel (0 to 7)
 * @param duty Duty cycle in percent (0 to 100)
 * @return int32_t 0 on success
 */
int32_t set_pwm_duty(void *p, uint8_t ch, uint8_t duty);

#endif // CONFIG_H