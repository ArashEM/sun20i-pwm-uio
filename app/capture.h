/**
 * @file capture.h
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief T133 PWM capture mode API
 * @version 0.1
 * @date 2024-09-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "soc.h"
#include "clk.h"
#include "registers.h"

/**
 * @brief Enable / Disable Capture interrupt
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7] 
 * @param rising Enable / Disable rising edge IRQ
 * @param falling Enable / Disable falling edge IRQ
 * @return int32_t 0 on success
 */
inline int32_t en_cap_irq(void *p, uint8_t ch, bool rising, bool falling)
{
    if(check_ch(ch))
        return -EINVAL;

    rmwb(p + CIER_OFFSET, CRIEx(ch), rising);
    rmwb(p + CIER_OFFSET, CFIEx(ch), falling);

    return 0;
}

/**
 * @brief Enable / Disable capture mode
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7] 
 * @param rising Enable / Disable rising edge capture mode
 * @param falling Enable / Disable falling edge capture mode
 * @return int32_t 
 * @note This function do not check if PWM mode is enabled or not!
 */
inline int32_t cap_en(void *p, uint8_t ch, bool rising, bool falling)
{
    if(check_ch(ch))
        return -EINVAL;

    rmwb(p + CER_OFFSET, CAPx_EN(ch), rising | falling);
    rmwb(p + PWM_REG_OFFSET(CCR_OFFSET, ch), CRTE, rising);
    rmwb(p + PWM_REG_OFFSET(CCR_OFFSET, ch), CFTE, falling);

    return 0;
}

/**
 * @brief Clear Falling or Rising edge IRQ flag
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]  
 * @param rising true: Clear rising edge IRQ flag
 * @param falling true: Clear rising edge IRQ flag
 * @return int32_t 
 */
inline int32_t clear_cap_irq(void *p, uint8_t ch, bool rising, bool falling)
{
    if(check_ch(ch))
        return -EINVAL;

    if(rising) {
        rmwb(p + CISR_OFFSET, CRISx(ch), false);
        rmwb(p + PWM_REG_OFFSET(CCR_OFFSET, ch), CRLF, false);
    }

    if(falling) {
        rmwb(p + CISR_OFFSET, CFISx(ch), false);
        rmwb(p + PWM_REG_OFFSET(CCR_OFFSET, ch), CFLF, false);
    }

    return 0;
}

/**
 * @brief Report current IRQ flag
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]  
 * @param rising Report rising edge flag
 * @param falling Report falling edge flag
 * @return int32_t 
 */
inline int32_t cap_irq(void *p, uint8_t ch, bool *rising, bool *falling)
{
    if(check_ch(ch))
        return -EINVAL;
    
    if(!rising | !falling)
        return -EFAULT;

    uint32_t reg = readl(p + CISR_OFFSET);

    *rising = IS_SET(reg, CRISx(ch));
    *falling = IS_SET(reg, CFISx(ch));

    return 0;
}

/**
 * @brief Report rising lock register 
 * 
 * @param p 
 * @param ch 
 * @param rlock 
 * @return int32_t 
 */
inline int32_t cap_rising_lock(void *p, uint8_t ch, uint16_t *rlock)
{
    if(check_ch(ch))
        return -EINVAL;

    if(!rlock)
        return -EFAULT;

    uint32_t reg = readl(p + PWM_REG_OFFSET(CRLR_OFFSET, ch));
    *rlock = CRLR(reg);

    return 0;
}

/**
 * @brief Report Fall lock register
 * 
 * @param p 
 * @param ch 
 * @param flock 
 * @return int32_t 
 */
inline int32_t cap_falling_lock(void *p, uint8_t ch, uint16_t *flock)
{
    if(check_ch(ch))
        return -EINVAL;

    if(!flock)
        return -EFAULT;

    uint32_t reg = readl(p + PWM_REG_OFFSET(CFLR_OFFSET, ch));
    *flock = CFLR(reg);

    return 0;
}