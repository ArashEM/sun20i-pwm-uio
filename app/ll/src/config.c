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

int32_t pwm_min_max_period(uint64_t *max_ns, uint64_t *min_ns)
{
    if(!max_ns | !min_ns)
        return -EFAULT;

    return 0;
}

int32_t pwm_clk_period(const struct pwm_config *config, uint64_t *period_ns)
{
    if(!config | !period_ns)
        return -EFAULT;

    if(check_clk(config->clk))
        return -EINVAL;

    uint32_t clk_freq = (config->clk.src == APB0 ? APB0_FREQ : HOSC_FREQ);
    uint16_t divider = ( 1 << config->clk.div );
    *period_ns = ( NSEC_IN_SEC / clk_freq ) * (1 + config->pre) * divider;
    
    return 0;
}

int32_t get_pwm_freq(void *p, uint8_t ch, uint64_t *freq_hz)
{
    if(!freq_hz)
        return -EFAULT;

    int32_t ret;
    struct pwm_config config;
    ret = get_pwm_config(p, ch, &config);
    if(ret)
        return ret;

    uint64_t period_ns;
    ret = pwm_clk_period(&config, &period_ns);
    if(ret)
        return ret;

    *freq_hz = NSEC_IN_SEC / (period_ns * (config.period.entire + 1));

    return 0;
}

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

    ret = get_clk_config(p, ch, &config->clk);
    if(ret)
        return ret;

    ret = get_prescaler(p, ch, &config->pre);
    if(ret)
        return ret;

    // ToDo: act_state must be implemented
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
    struct pwm_config config;
    ret = get_pwm_config(p, ch, &config);
    if(ret)
        return ret;

    // how long each cycle is?
    uint64_t clk_period;
    ret = pwm_clk_period(&config, &clk_period);

    result->on_ns = raw->on_cycles * clk_period;
    result->off_ns = raw->off_cycles * clk_period;

    return 0;
}

int32_t cap_max_duration(void *p, uint8_t ch, uint64_t *max_ns)
{
    struct cap_result_raw raw = {.on_cycles = 65535 };
    struct cap_result res;

    int32_t ret = result_to_ns(p, ch, &raw, &res);
    if(ret)
        return ret;

    *max_ns = res.on_ns;

    return 0;
}