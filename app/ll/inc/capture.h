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

/**
 * @brief Enable / Disable Capture interrupt
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7] 
 * @param rising Enable / Disable rising edge IRQ
 * @param falling Enable / Disable falling edge IRQ
 * @return int32_t 0 on success
 */
int32_t en_cap_irq(void *p, uint8_t ch, bool rising, bool falling);

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
int32_t cap_en(void *p, uint8_t ch, bool rising, bool falling);

/**
 * @brief Clear Falling or Rising edge IRQ flag
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]  
 * @param rising true: Clear rising edge IRQ flag
 * @param falling true: Clear rising edge IRQ flag
 * @return int32_t 
 */
int32_t clear_cap_irq(void *p, uint8_t ch, bool rising, bool falling);

/**
 * @brief Report current IRQ flag
 * 
 * @param base Base address of PWM peripheral
 * @param ch Channel index [0, 7]  
 * @param rising Report rising edge flag
 * @param falling Report falling edge flag
 * @return int32_t 
 */
int32_t cap_irq(void *p, uint8_t ch, bool *rising, bool *falling);

/**
 * @brief Report rising lock register 
 * 
 * @param p 
 * @param ch 
 * @param rlock 
 * @return int32_t 
 */
int32_t cap_rising_lock(void *p, uint8_t ch, uint16_t *rlock);

/**
 * @brief Report Fall lock register
 * 
 * @param p 
 * @param ch 
 * @param flock 
 * @return int32_t 
 */
int32_t cap_falling_lock(void *p, uint8_t ch, uint16_t *flock);

/**
 * @brief Report capture rise lock flag (from CCR)
 * 
 * @param p 
 * @param ch 
 * @param crlf Capture Rise Lock Flag
 * @return int32_t 0 on success
 * @note This API do not need interrupt to be enabled
 */
int32_t cap_crlf(void *p, uint8_t ch, bool *crlf);

/**
 * @brief Report capture fall lock flag
 * 
 * @param p 
 * @param ch 
 * @param cflf Capture Fall Lock Flag
 * @return int32_t 0 on success
 */
int32_t cap_cflf(void *p, uint8_t ch, bool *cflf);