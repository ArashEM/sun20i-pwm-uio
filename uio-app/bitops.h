#ifndef BITOPS_H
#define BITOPS_H
/**
 * @brief Bit operations macros
 * @note Please check include/linux/bitops.h for more information
 */

#define BIT(nr)             ( 1UL << (nr) )
#define CLEAR_BIT(x, nr)    ( (x) &= ~BIT(nr) )
#define SET_BIT(x, nr)      ( (x) |= BIT(nr) )

#endif // BITOPS_H