#pragma once
#include <stdint.h>
#include "rcc.h"

#define SYSTICK ((SysTick_Regs *)0xE000E010U)

#define SYSTICK_LOAD_VALUE ((AHB_CLK / 1000) - 1)

void SysTick_Init();
void SysTick_Delay_Microseconds(uint32_t microseconds);
void SysTick_Delay_Milliseconds(uint32_t milliseconds);
uint32_t SysTick_Get_Milliseconds();
uint32_t SysTick_Get_Microseconds();

extern volatile uint32_t systick_ms;

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_Regs;

enum SysTick_CTRL_Bits {
    SYSTICK_CTRL_ENABLE = (1 << 0),
    SYSTICK_CTRL_TICKINT = (1 << 1),
    SYSTICK_CTRL_CLKSOURCE_EXTERNAL = 0,   // external source is AHB/8 on stm32f411
    SYSTICK_CTRL_CLKSOURCE_INTERNAL = (1 << 2) // internal processor clock
};
