/**
 * @file capture.c
*  @author Arash Golgol (arash.golgol@gmail.com)
 * @brief T133 PWM capture mode API 
 * @version 0.1
 * @date 2024-09-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "soc.h"
#include "clk.h"
#include "registers.h"
#include "bitops.h"
#include "rw.h" 
#include "capture.h"

 int32_t en_cap_irq(void *p, uint8_t ch, bool rising, bool falling)
{
    if(check_ch(ch))
        return -EINVAL;

    rmwb(p + CIER_OFFSET, CRIEx(ch), rising);
    rmwb(p + CIER_OFFSET, CFIEx(ch), falling);

    return 0;
}

 int32_t cap_en(void *p, uint8_t ch, bool rising, bool falling)
{
    if(check_ch(ch))
        return -EINVAL;

    rmwb(p + CER_OFFSET, CAPx_EN(ch), rising | falling);
    rmwb(p + PWM_REG_OFFSET(CCR_OFFSET, ch), CRTE, rising);
    rmwb(p + PWM_REG_OFFSET(CCR_OFFSET, ch), CFTE, falling);

    return 0;
}

 int32_t clear_cap_irq(void *p, uint8_t ch, bool rising, bool falling)
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

 int32_t cap_irq(void *p, uint8_t ch, bool *rising, bool *falling)
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

 int32_t cap_rising_lock(void *p, uint8_t ch, uint16_t *rlock)
{
    if(check_ch(ch))
        return -EINVAL;

    if(!rlock)
        return -EFAULT;

    uint32_t reg = readl(p + PWM_REG_OFFSET(CRLR_OFFSET, ch));
    *rlock = CRLR(reg);

    return 0;
}

 int32_t cap_falling_lock(void *p, uint8_t ch, uint16_t *flock)
{
    if(check_ch(ch))
        return -EINVAL;

    if(!flock)
        return -EFAULT;

    uint32_t reg = readl(p + PWM_REG_OFFSET(CFLR_OFFSET, ch));
    *flock = CFLR(reg);

    return 0;
}

 int32_t cap_crlf(void *p, uint8_t ch, bool *crlf)
{
    if(check_ch(ch))
        return -EINVAL;

    if(!crlf)
        return -EFAULT;

    uint32_t reg = readl(p + PWM_REG_OFFSET(CCR_OFFSET, ch));
    *crlf = IS_SET(reg, CRLF);

    return 0;
}

 int32_t cap_cflf(void *p, uint8_t ch, bool *cflf)
{
    if(check_ch(ch))
        return -EINVAL;

    if(!cflf)
        return -EFAULT;

    uint32_t reg = readl(p + PWM_REG_OFFSET(CCR_OFFSET, ch));
    *cflf = IS_SET(reg, CFLF);

    return 0;
}