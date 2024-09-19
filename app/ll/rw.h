#ifndef RW_H
#define RW_H
/**
 * @brief Read / Write helper functions
 * 
 */
#include <stdint.h>
#include <stdbool.h>

void writel(void* base, uint32_t value);

uint32_t readl(void* base);

/**
 * @brief Read, Modify and Write single bit in 32bit
 * 
 * @param base Base address 
 * @param index Index of bit [0, 31]
 * @param bit Desired value of target bit
 */
void rmwb(void *base, uint8_t index, bool bit);   

#endif // RW_H