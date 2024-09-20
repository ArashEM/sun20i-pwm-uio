/**
 * @file soc.c
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-09-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdint.h>
#include <errno.h>

#include "soc.h"

int32_t check_ch(uint8_t ch) 
{
    return (ch < PWM_CHANNEL) ? 0 : -EINVAL;
}