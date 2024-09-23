#ifndef CLK_H
#define CLK_H
/**
 * @file clk.h
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief T113-S3 PWM/Capture clock configuration
 * @version 0.1
 * @date 2024-09-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

/**
 * @brief PWM clock source
 * 
 */
enum clk_src {
    HOSC = 0,       // 24Mhz crystal oscillator
    APB0 = 1        // 100Mhz APB0 bus clock
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

/**
 * @brief Check source and divider range
 * 
 * @param clk 
 * @return int32_t 0 on success
 */
int32_t check_clk(struct pwm_clk clk);

/**
 * @brief Pass / Mask PWM clock
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]
 * @param pass true: Pass the clock, false: gate the clock
 * @return int32_t 0 on success
 */
int32_t clk_gate(void *base, uint8_t ch, bool pass);

/**
 * @brief Configure PWM/Capture clock 
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7] 
 * @param clk Clock configuration (Source and Divider)
 * @return int32_t 0 on success
 * @note It's shared between (c) and (ch + 1). Please check reference manual
 */
int32_t clk_config(void *base, uint8_t ch, struct pwm_clk clk);

/**
 * @brief Report current clock configuration
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]  
 * @param clk Pointer clock configuration to be filled
 * @return int32_t 0 on success
 */
int32_t get_clk_config(void *base, uint8_t ch, struct pwm_clk *clk);

/**
 * @brief Set PWM/Capture pre scaler
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7] 
 * @param pre Clock pre scaler
 * @return int32_t 0 on success
 */
int32_t set_prescaler(void *base, uint8_t ch, uint8_t pre);

/**
 * @brief Report current value of pre-scaler
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]  
 * @param pre Pre scaler value to be filled
 * @return int32_t 0 on success
 */
int32_t get_prescaler(void *base, uint8_t ch, uint8_t *pre);


#endif // CLK_H