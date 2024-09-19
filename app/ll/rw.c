/**
 * @file rw.c
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief Read / Write helper functions
 * @version 0.1
 * @date 2024-09-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "rw.h"
#include "bitops.h"

void writel(void* base, uint32_t value)
{
    uint32_t *reg = (uint32_t *)base;
    *reg = value;
}

uint32_t readl(void* base)
{
    uint32_t *reg = (uint32_t *)base;
    return *reg;
}

void rmwb(void *base, uint8_t index, bool bit)
{
    if(index > 31)
        return;

    uint32_t reg = readl(base);
    if(bit)
        SET_BIT(reg, index);
    else
        CLEAR_BIT(reg, index);
    writel(base, reg);
}