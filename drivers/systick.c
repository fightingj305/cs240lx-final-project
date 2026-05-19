#include "systick.h"


void SysTick_Init() {
        SYSTICK->CTRL = 0;
        SYSTICK->LOAD = SYSTICK_LOAD_VALUE;
        SYSTICK->VAL = 0;
        SYSTICK->CTRL = SYSTICK_CTRL_CLKSOURCE_EXTERNAL | SYSTICK_CTRL_ENABLE;
}

uint32_t SysTick_Get_Microseconds(void) {
    static uint32_t accumulated_us = 0;
    static uint32_t last_val = 0;
    
    uint32_t now = SYSTICK->VAL;
    uint32_t load = SYSTICK_LOAD_VALUE + 1;
    uint32_t ticks_per_us = (AHB_CLK / 4) / 1000000 / 2;
    uint32_t elapsed_ticks;

    if (last_val >= now) {
        elapsed_ticks = last_val - now;
    } else {
        elapsed_ticks = last_val + (load - now);
    }

    accumulated_us += elapsed_ticks / ticks_per_us;
    last_val = now;
    return accumulated_us;
}

void SysTick_Delay_Microseconds(uint32_t microseconds) {
    uint32_t start = SYSTICK->VAL;
    uint32_t ticks = (AHB_CLK/4 / 1000000) * microseconds / 2;

    uint32_t load = SYSTICK_LOAD_VALUE + 1;
    uint32_t elapsed = 0;

    while (elapsed < ticks)
    {
        uint32_t now = SYSTICK->VAL;
        if (start >= now) {
            elapsed += start - now;
        }
        else {
            elapsed += start + (load - now);
        }
        start = now;
    }
}

void SysTick_Delay_Milliseconds(uint32_t milliseconds) {
    SysTick_Delay_Microseconds(milliseconds * 1000);
}