#include "rcc.h"
#include "flash.h"

void RCC_Init() {
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSE | // 25 MHz external crystal
                   (25 << RCC_PLLCFGR_PLLM_BIT) | // VCO input = 25 MHz / 25 = 1 MHz
                   (336 << RCC_PLLCFGR_PLLN_BIT) |  // VCO output = 1 MHz * 336 = 336 MHz
                   RCC_PLLCFGR_PLLP_DIV4 |  // Main PLL output = 336 MHz / 4 = 84 MHz
                   (7 << RCC_PLLCFGR_PLLQ_BIT); // USB clock = 336 MHz / 7 = 48 MHz
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    FLASH->ACR = FLASH_ACR_LATENCY_2WS; // 2 wait states for 84 MHz at highest voltage level

    RCC->CFGR = RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1; // AHB=84 MHz, APB1=42 MHz, APB2=84 MHz
    RCC->CFGR |= RCC_CFGR_SW_PLL; // system clock to PLL
    while ((RCC->CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL);

    RCC->CR &= ~RCC_CR_HSION;
    while (RCC->CR & RCC_CR_HSIRDY);
}