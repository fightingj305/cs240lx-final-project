#include "systick.h"
#include "utils.h"

volatile uint32_t systick_ms = 0;

void SysTick_Init() {
        SYSTICK->CTRL = 0;
        SYSTICK->LOAD = SYSTICK_LOAD_VALUE;
        SYSTICK->VAL = 0;
        SYSTICK->CTRL = SYSTICK_CTRL_CLKSOURCE_INTERNAL  | SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_TICKINT;
}

void SysTick_Handler(void) {
    systick_ms++;
}

uint32_t SysTick_Get_Milliseconds(void) {
  return systick_ms;
}

uint32_t SysTick_Get_Microseconds() {
    uint32_t ms  = systick_ms;
    uint32_t val = SYSTICK->VAL;
    if (systick_ms != ms) {
        ms  = systick_ms;
        val = SYSTICK->VAL;
    }
    uint32_t ticks_per_us = AHB_CLK / 1000000;
    uint32_t us_in_ms = (SYSTICK_LOAD_VALUE - val) / ticks_per_us;
    return ms * 1000 + us_in_ms;
}
void SysTick_Delay_Microseconds(uint32_t microseconds) {
    uint32_t ticks_per_us = AHB_CLK / 1000000;
    uint32_t ticks = ticks_per_us * microseconds;
    uint32_t load = SYSTICK_LOAD_VALUE + 1;
    uint32_t start = SYSTICK->VAL;
    uint32_t elapsed = 0;

    while (elapsed < ticks) {
        uint32_t now = SYSTICK->VAL;
        if (start >= now) {
            elapsed += start - now;
        } else {
            elapsed += start + (load - now);
        }
        start = now;
    }
}

void SysTick_Delay_Milliseconds(uint32_t milliseconds) {
    SysTick_Delay_Microseconds(milliseconds * 1000);
}