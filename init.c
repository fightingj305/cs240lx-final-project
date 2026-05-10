#include <stdint.h>
#include "scb.h"
#include "memmap.h"
#include "rcc.h"

void SystemInit(void)
{
    // Coprocessor for FPU
    SCB->CPACR |= SCB_CPACR_CP10_FULL | SCB_CPACR_CP11_FULL;

    // Vector table base
    SCB->VTOR = VECTOR_BASE;

    RCC_Init();
}