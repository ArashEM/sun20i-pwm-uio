#ifndef PWM_TYPES_H
#define PWM_TYPES_H
/**
 * @brief Type definitions for T113 PWM register access
 * 
 */
#include <stdint.h>
#include <stdbool.h>

// PWM Clock Configuration

/**
 * @brief PWM clock source
 * 
 */
enum clk_src {
    HOSC = 0,       // 24Mhz crystal oscillator
    APB0 = 1        // 100Mhz APBO bus clock
};

/**
 * @brief PWM clock divider 
 * @note It's shared between (ch) and (ch + 1)
 */
enum clk_div {
    DIV_1 =     0x00,
    DIV_2 =     0x01,
    DIV_4 =     0x02,
    DIV_8 =     0x03,
    DIV_16 =    0x04,
    DIV_32 =    0x05,
    DIV_64 =    0x06,
    DIV_128 =   0x07,
    DIV_256 =   0x08
};

/**
 * @brief Input clock configuration
 * 
 */
struct pwm_clk {
    enum clk_src src;
    enum clk_div div;
};

#endif // PWM_TYPES_H