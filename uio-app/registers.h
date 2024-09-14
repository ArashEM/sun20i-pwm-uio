#ifndef REGISTERS_H
#define REGISTERS_H
/**
 * @brief T133-Sx PWM registers
 * @note Check t113-s3_user_manual_v1.3_.pdf for more information
 */

#include "bitops.h"

#define PIER_OFFSET         0x0000 // PWM IRQ Enable Register
#define PISR_OFFSET         0x0004 // PWM IRQ Status Register
#define CIER_OFFSET         0x0010 // Capture IRQ Enable Register
#define CISR_OFFSET         0x0014 // Capture IRQ Status Register
#define PCCR01_OFFSET       0x0020 // PWM01 Clock Configuration Register
#define PCCR23_OFFSET       0x0024 // PWM23 Clock Configuration Register
#define PCCR45_OFFSET       0x0028 // PWM45 Clock Configuration Register
#define PCCR57_OFFSET       0x002C // PWM67 Clock Configuration Register
#define PCGR_OFFSET         0x0040 // PWM Clock Gating Register
#define PDZCR01_OFFSET      0x0060 // PWM01 Dead Zone Control Register 
#define PDZCR23_OFFSET      0x0064 // PWM23 Dead Zone Control Register
#define PDZCR45_OFFSET      0x0068 // PWM45 Dead Zone Control Register
#define PDZCR67_OFFSET      0x006C // PWM67 Dead Zone Control Register
#define PER_OFFSET          0x0080 // PWM Enable Register
#define PGR0_OFFSET         0x0090 // PWM Group0 Register
#define PGR1_OFFSET         0x0094 // PWM Group1 Register
#define PGR2_OFFSET         0x0098 // PWM Group2 Register
#define PGR3_OFFSET         0x009C // PWM Group3 Register
#define CER_OFFSET          0x00C0 // Capture Enable Register
#define PCR_OFFSET          0x0100 // PWM Control Register
#define PPR_OFFSET          0x0104 // PWM Period Register
#define PCNTR_OFFSET        0x0108 // PWM Count Register
#define PPCNTR_OFFSET       0x010C // PWM Pulse Count Register
#define CCR_OFFSET          0x0110 // Capture Control Register
#define CRLR_OFFSET         0x0114 // Capture Rise Lock Register
#define CFLR_OFFSET         0x0118 // Capture Fall Lock Register

#define PWM_REG_OFFSET(base, ch) ((base) + 0x20 * (ch))

// PWM Clock Configuration Register details
#define PWM_CLK_SRC_SEL         0x07
#define PCCRxy_OFFSET(ch)       (PCCR01_OFFSET + 0x04 *(ch > 1))
#define PCCRxy_VALUE(src, div)  ( ((src) << PWM_CLK_SRC_SEL) | (div) )

// PWM Clock Gating Register details
#define PWMx_CLK_BYPASS(x)      ((x) + 16)
#define PWMx_CLK_GATING(x)      (x)

// PWM Enable Register details
#define PWMx_EN(x)              (x)

// PWM Control Register
#define PWM_ACT_STA             0x08
#define PWM_PRESCAL_K_MASK      0xFF
#define SET_PWM_PRESCALE(x, pre)    \
do {                                \
    (x) &= ~PWM_PRESCAL_K_MASK;     \
    (x) |= pre;                     \
} while(0)

// PWM Period Register
#define PWM_ENTIRE_CYCLE        0x10
#define SET_PWM_PERIOD(x, ent, act) \
do {                                \
    (x) = (act);                    \
    (x) |= ((ent) << PWM_ENTIRE_CYCLE); \
} while(0)

#endif // REGISTERS_H