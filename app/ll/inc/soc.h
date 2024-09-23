#ifndef SOC_H
#define SOC_H
/**
 * @file soc.h
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief T113 SoC related definitions
 * @version 0.1
 * @date 2024-09-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdint.h>

#define HOSC_FREQ       24000000UL      // Main clock frequency (Hz)
#define APB0_FREQ       100000000UL     // APB0 bus clock frequency (Hz)

#define NSEC_IN_SEC     1000000000UL    // nS in S
#define MSEC_IN_NSEC    1000UL          // uS in S

#define PWM_CHANNEL     0x08

/**
 * @brief Check PWM channel range
 * 
 * @param ch 
 * @return int32_t 0 on success
 */
int32_t check_ch(uint8_t ch);

#endif // SOC_H