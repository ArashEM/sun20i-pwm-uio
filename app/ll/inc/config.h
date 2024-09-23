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
#include "capture.h"

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
 * @brief Overall configuration of capture channel
 * 
 */
struct cap_config
{
    struct pwm_clk clk;         // clock source and divider
    uint8_t pre;                // pre-scaler
    bool rising;                // capture rising edge
    bool falling;               // capture falling edge
};

/**
 * @brief Report capture result in number of cycles
 * 
 */
struct cap_result_raw
{
    uint16_t on_cycles;
    uint16_t off_cycles;
};

/**
 * @brief Capture result in nano seconds
 * 
 */
struct cap_result 
{
    uint64_t on_ns;
    uint64_t off_ns;
};

/**
 * @brief Absolute minium and maximum period of PWM output
 * 
 * @param max_ns Maximum period in nS
 * @param min_ns Minium period in nS
 * @return int32_t 0 on success
 */
int32_t pwm_min_max_period(uint64_t *max_ns, uint64_t *min_ns);

/**
 * @brief PWM pulse counter clock period (which increment PPCNTR)
 * 
 * @param config Pointer to PWM configuration
 * @param period_ns Clock period in nS
 * @return int32_t 0 on success
 */
int32_t pwm_clk_period(const struct pwm_config *config, uint64_t *period_ns);

/**
 * @brief Report PWM output frequency
 * 
 * @param p Pointer to PWM base address
 * @param ch PWM channel (0 to 7) 
 * @param freq_hz PWM frequency in Hertz 
 * @return int32_t 0 on success
 */
int32_t get_pwm_freq(void *p, uint8_t ch, uint64_t *freq_hz);

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

/**
 * @brief Apply Capture configuration in it's registers
 * 
 * @param p Pointer to PWM base address
 * @param ch PWM channel (0 to 7) 
 * @param config Pointer to configuration
 * @return int32_t 0 on success
 */
int32_t set_cap_config(void *p, uint8_t ch, const struct cap_config *config);

/**
 * @brief Capture on/off cycles of input pules in blocking mode
 * 
 * @param p Pointer to PWM base address
 * @param ch PWM channel (0 to 7)  
 * @param result Number of cycles of on and off
 * @return int32_t 0 on success
 */
int32_t cap_blocking(void *p, uint8_t ch, struct cap_result_raw *result);

/**
 * @brief Convert capture raw results into nS
 * 
 * @param p Pointer to PWM base address
 * @param ch PWM channel (0 to 7)  
 * @param raw Raw results in number of cycles
 * @param result Results in nS
 * @return int32_t 0 on success
 */
int32_t result_to_ns(void *p, uint8_t ch, 
                     const struct cap_result_raw *raw,
                     struct cap_result *result);

/**
 * @brief Report maximum duration capture can record based 
 *        on it'c clock configuration
 * 
 * @param p Pointer to PWM base address
 * @param ch PWM channel (0 to 7) 
 * @param max_ns Maximum On/Off duration capture can measure in nS
 * @return int32_t 
 */
int32_t cap_max_duration(void *p, uint8_t ch, uint64_t *max_ns);

#endif // CONFIG_H