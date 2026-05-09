#include <stdint.h>
#include "scb.h"
#include "rcc.h"

void SystemInit(void)
{
    // Coprocessor for FPU
    SCB->CPACR |= SCB_CPACR_CP10_FULL | SCB_CPACR_CP11_FULL;

    // Vector table base, should this be a define?
    SCB->VTOR = 0x08000000;

    RCC_Init();
}