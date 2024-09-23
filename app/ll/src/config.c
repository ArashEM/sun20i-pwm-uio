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
#include <unistd.h>
#include <stdio.h>
#include "soc.h"
#include "config.h"

int32_t pwm_calc(uint64_t period_ns, uint8_t duty_cycle, struct pwm_config *config)
{
    (void)period_ns;
    (void)duty_cycle;
    (void)config;
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

    // ToDo: Check if capture mode is enabled or not!

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

int32_t set_cap_config(void *p, uint8_t ch, const struct cap_config *config)
{
    int32_t ret;

    if(!config)
        return -EFAULT;

    // Don't touch if PWM is enabled
    bool pwm = false;
    ret = is_pwm_en(p, ch, &pwm);
    if(ret)
        return ret;

    if(pwm)
        return -EBUSY;

    ret = clk_gate(p, ch, config->rising & config->falling);
    if(ret)
        return ret;

    ret = clk_config(p, ch, config->clk);
    if(ret)
        return ret;

    ret = set_prescaler(p, ch, config->pre);
    if(ret)
        return ret;

    ret = cap_en(p, ch, config->rising, config->falling);
    if(ret)
        return ret;

    ret = clear_cap_irq(p, ch, true, true);
    if(ret)
        return ret;

    return 0;
}

int32_t cap_blocking(void *p, uint8_t ch, struct cap_result_raw *result)
{
    if(!result)
        return -EFAULT;

    bool loop = true;
    while( loop ) {
        bool cflf;
        cap_cflf(p, ch, &cflf);

        bool crlf;
        cap_crlf(p, ch, &crlf);

        if(crlf & cflf) {
            cap_rising_lock(p, ch, &result->off_cycles);
            cap_falling_lock(p, ch, &result->on_cycles);
            clear_cap_irq(p, ch, true, true);
            loop = false;
        }
        usleep(1000*100);
    }

    return 0;
}

int32_t result_to_ns(void *p, uint8_t ch, 
                     const struct cap_result_raw *raw,
                     struct cap_result *result)
{
    if(!raw || !result)
        return -EFAULT;

    int32_t ret;
    struct pwm_clk clk;
    ret = get_clk_config(p, ch, &clk);
    if(ret)
        return ret;

    uint8_t pre;
    ret = get_prescaler(p, ch, &pre);
    if(ret)
        return ret;

    uint32_t clk_freq = (clk.src == APB0 ? APB0_FREQ : HOSC_FREQ);
    uint16_t divider = ( 1 << clk.div );
    uint64_t factor = ( NSEC_IN_SEC / clk_freq ) * (1 + pre) * divider;

    result->on_ns = raw->on_cycles * factor;
    result->off_ns = raw->off_cycles * factor;

    return 0;
}