#ifndef MM32F3277_SYSTEM_SYSTEM_H_
#define MM32F3277_SYSTEM_SYSTEM_H_

#include "hal_conf.h"
#include  "stdio.h"

#define COREID_MASK             (0x00000070U)

////////////////////////////////////////////////////////////////////////////////
/// @brief  System clock configuration
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    SYSTEMCLK_HSI_8MHz = 0,
    SYSTEMCLK_HSI_12MHz,
    SYSTEMCLK_HSI_24MHz,
    SYSTEMCLK_HSI_48MHz,
    SYSTEMCLK_HSI_72MHz,
    SYSTEMCLK_HSI_96MHz,
    SYSTEMCLK_HSE_8MHz,
    SYSTEMCLK_HSE_12MHz,
    SYSTEMCLK_HSE_24MHz,
    SYSTEMCLK_HSE_48MHz,
    SYSTEMCLK_HSE_72MHz,
    SYSTEMCLK_HSE_96MHz,
    SYSTEMCLK_HSEDIV2_4MHz,
    SYSTEMCLK_HSEDIV2_8MHz,
    SYSTEMCLK_HSEDIV2_12MHz,
    SYSTEMCLK_HSEDIV2_24MHz,
    SYSTEMCLK_HSEDIV2_48MHz,
    SYSTEMCLK_HSEDIV2_72MHz,
    SYSTEMCLK_HSEDIV2_96MHz,
    SYSTEMCLK_LSI_40KHz,
#if defined(__CUSTOM_FREQ)
    SYSTEMCLK_CUSTOM_Freq,
#endif
} SYSTEMCLK_TypeDef;
#define RCC_LATENCY_TB         0
#define RCC_PLLMUL_TB          1
#define RCC_PLLDIV_TB          2
#define RCC_SYSCLKSRC_TB       3
#define RCC_PLLSRC_TB          4
#define RCC_ROW_TB             5

#ifdef _SYS_C_
#define GLOBAL

GLOBAL u32 gSystemClockValue = 8000000UL;
#else
#define GLOBAL extern

GLOBAL u32 gSystemClockValue;

#endif

#undef GLOBAL

void SystemReInit(SYSTEMCLK_TypeDef system_clock);

#endif  // MM32F3277_SYSTEM_SYSTEM_H_
