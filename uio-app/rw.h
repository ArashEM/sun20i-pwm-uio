#ifndef RW_H
#define RW_H
/**
 * @brief Read / Write helper functions
 * 
 */
#include <stdint.h>
#include <stdbool.h>

#include "bitops.h"

inline void writel(void* base, uint32_t value)
{
    uint32_t *reg = (uint32_t *)base;
    *reg = value;
}

inline uint32_t readl(void* base)
{
    uint32_t *reg = (uint32_t *)base;
    return *reg;
}

/**
 * @brief Read, Modify and Write single bit in 32bit
 * 
 * @param base Base address 
 * @param index Index of bit [0, 31]
 * @param bit Desired value of target bit
 */
inline void rmwb(void *base, uint8_t index, bool bit)
{
    if(index > 31)
        return;

    uint32_t tmp = readl(base);
    if(bit)
        tmp |= BIT(index);
    else
        tmp &= ~BIT(index);
    writel(base, tmp);
}     

#endif // RW_H