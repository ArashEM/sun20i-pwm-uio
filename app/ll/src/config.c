/**
 * @file config.c
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief PWM / Capture configurations (high level API)
 * @version 0.1
 * @date 2024-09-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <errno.h>
#include "config.h"

int32_t pwm_calc(uint64_t period_ns, uint8_t duty_cycle, struct pwm_config *config)
{
    return 0;
}

int32_t get_pwm_config(void *p, uint8_t ch, struct pwm_config *config)
{
    int32_t ret;
    
    if(!config)
        return -EFAULT;

    ret = is_pwm_en(p, ch, &config->en);
    if(ret)
        return ret;

    ret = get_period(p, ch, &config->period);
    if(ret)
        return ret;

    // ToDo: clk, pre-scaler and act_state must be implemented
    return 0;
}

int32_t set_pwm_config(void *p, uint8_t ch, const struct pwm_config *config)
{
    int32_t ret;
    
    if(!config)
        return -EFAULT;

    /**
     * @brief In case of disable, we need clock to be gated after
     *        PWM cycle is finished! So we disable it at the end
     */
    if(config->en) {
        ret = clk_gate(p, ch, true);
        if(ret)
            return ret;
    }

    ret = clk_config(p, ch, config->clk);
    if(ret)
        return ret;

    ret = set_period(p, ch, config->period);
    if(ret)
        return ret;

    ret = set_prescaler(p, ch, config->pre);
    if(ret)
        return ret;

    ret = set_act_state(p, ch, config->state);
    if(ret)
        return ret;

    ret = pwm_en(p, ch, config->en);
    if(ret)
        return ret;

    /**
     * @brief In case of disabling, clock is gated after PWM is disabled.
     *        Check pwm_en() for more information
     */
    if(!config->en) {
        ret = clk_gate(p, ch, false);
        if(ret)
            return ret;
    }

    return 0;
}

int32_t set_pwm_duty(void *p, uint8_t ch, uint8_t duty)
{
    int32_t ret;
    struct pwm_period period;

    if(duty > 100)
        return -EINVAL;

    ret = get_period(p, ch, &period);
    if(ret)
        return ret;

    period.act = (period.entire * duty) / 100;
    ret = set_period(p, ch, period);
    
    return ret;
}