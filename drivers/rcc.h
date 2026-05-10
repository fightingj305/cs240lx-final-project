#pragma once
#include <stdint.h>

#define APB1_CLK 42000000U
#define APB2_CLK 84000000U
#define AHB_CLK 84000000U
#define SYS_CLK 84000000U
#define HSI_CLK 16000000U
#define HSE_CLK 25000000U

#define RCC ((RCC_Regs *)0x40023800U)

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    uint32_t RESERVED0[2];
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    uint32_t RESERVED2[2];
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    // ignore the rest of the registers for now
} RCC_Regs;

void RCC_Init(void);

// AHB1 peripheral clock enable register (RCC_AHB1ENR)
enum RCC_AHB1_ENR_Bits {
    RCC_AHB1ENR_GPIOA_EN = (1 << 0),
    RCC_AHB1ENR_GPIOB_EN = (1 << 1),
    RCC_AHB1ENR_GPIOC_EN = (1 << 2),
    RCC_AHB1ENR_GPIOD_EN = (1 << 3),
    RCC_AHB1ENR_GPIOE_EN = (1 << 4),
    RCC_AHB1ENR_GPIOH_EN = (1 << 7),
    RCC_AHB1ENR_CRC_EN   = (1 << 12),
    RCC_AHB1ENR_DMA1_EN  = (1 << 21),
    RCC_AHB1ENR_DMA2_EN  = (1 << 22)
};

// AHB2 peripheral clock enable register (RCC_AHB2ENR)
enum RCC_AHB2_ENR_Bits {
    RCC_AHB2ENR_OTGFS_EN = (1 << 7)
};

// APB1 peripheral clock enable register (RCC_APB1ENR)
enum RCC_APB1_ENR_Bits {
    RCC_APB1ENR_TIM2_EN   = (1 << 0),
    RCC_APB1ENR_TIM3_EN   = (1 << 1),
    RCC_APB1ENR_TIM4_EN   = (1 << 2),
    RCC_APB1ENR_TIM5_EN   = (1 << 3),
    RCC_APB1ENR_WWDG_EN   = (1 << 11),
    RCC_APB1ENR_SPI2_EN   = (1 << 14),
    RCC_APB1ENR_SPI3_EN   = (1 << 15),
    RCC_APB1ENR_USART2_EN = (1 << 17),
    RCC_APB1ENR_I2C1_EN   = (1 << 21),
    RCC_APB1ENR_I2C2_EN   = (1 << 22),
    RCC_APB1ENR_I2C3_EN   = (1 << 23),
    RCC_APB1ENR_PWR_EN    = (1 << 28)
};

// APB2 peripheral clock enable register (RCC_APB2ENR)
enum RCC_APB2_ENR_Bits {
    RCC_APB2ENR_TIM1_EN   = (1 << 0),
    RCC_APB2ENR_USART1_EN = (1 << 4),
    RCC_APB2ENR_USART6_EN = (1 << 5),
    RCC_APB2ENR_ADC1_EN   = (1 << 8),
    RCC_APB2ENR_SDIO_EN   = (1 << 11),
    RCC_APB2ENR_SPI1_EN   = (1 << 12),
    RCC_APB2ENR_SPI4_EN   = (1 << 13),
    RCC_APB2ENR_SYSCFG_EN = (1 << 14),
    RCC_APB2ENR_TIM9_EN   = (1 << 16),
    RCC_APB2ENR_TIM10_EN  = (1 << 17),
    RCC_APB2ENR_TIM11_EN  = (1 << 18),
    RCC_APB2ENR_SPI5_EN   = (1 << 20)
};

// Clock control register (RCC_CR)
enum RCC_CR_Bits {
    RCC_CR_HSION     = (1 << 0),
    RCC_CR_HSIRDY    = (1 << 1),
    RCC_CR_HSITRIM_0 = (1 << 3),
    RCC_CR_HSEON     = (1 << 16),
    RCC_CR_HSERDY    = (1 << 17),
    RCC_CR_HSEBYP    = (1 << 18),
    RCC_CR_CSSON     = (1 << 19),
    RCC_CR_PLLON     = (1 << 24),
    RCC_CR_PLLRDY    = (1 << 25),
    RCC_CR_PLLI2SON  = (1 << 26),
    RCC_CR_PLLI2SRDY = (1 << 27)
};

// PLL configuration register (RCC_PLLCFGR)
enum RCC_PLLCFGR_Bits {
    RCC_PLLCFGR_PLLSRC_HSI = (0 << 22),
    RCC_PLLCFGR_PLLSRC_HSE = (1 << 22),
    RCC_PLLCFGR_PLLM_BIT   = 0,
    RCC_PLLCFGR_PLLM_MASK  = (0x3F << RCC_PLLCFGR_PLLM_BIT),
    RCC_PLLCFGR_PLLN_BIT   = 6,
    RCC_PLLCFGR_PLLN_MASK  = (0x1FF << RCC_PLLCFGR_PLLN_BIT),

    RCC_PLLCFGR_PLLP_DIV2  = (0 << 16),
    RCC_PLLCFGR_PLLP_DIV4  = (1 << 16),
    RCC_PLLCFGR_PLLP_DIV6  = (2 << 16),
    RCC_PLLCFGR_PLLP_DIV8  = (3 << 16),

    RCC_PLLCFGR_PLLP_MASK  = (0x3 << 16),
    RCC_PLLCFGR_PLLQ_BIT   = 24,
    RCC_PLLCFGR_PLLQ_MASK  = (0xF << RCC_PLLCFGR_PLLQ_BIT)
};

// Clock configuration register (RCC_CFGR)
enum RCC_CFGR_Bits {
    // System clock switch
    RCC_CFGR_SW_HSI    = (0 << 0),
    RCC_CFGR_SW_HSE    = (1 << 0),
    RCC_CFGR_SW_PLL    = (2 << 0),
    RCC_CFGR_SW_MASK   = (3 << 0),
    
    // System clock switch status
    RCC_CFGR_SWS_HSI   = (0 << 2),
    RCC_CFGR_SWS_HSE   = (1 << 2),
    RCC_CFGR_SWS_PLL   = (2 << 2),
    RCC_CFGR_SWS_MASK  = (3 << 2),
    
    // AHB prescaler
    RCC_CFGR_HPRE_DIV1   = (0 << 4),
    RCC_CFGR_HPRE_DIV2   = (8 << 4),
    RCC_CFGR_HPRE_DIV4   = (9 << 4),
    RCC_CFGR_HPRE_DIV8   = (10 << 4),
    RCC_CFGR_HPRE_DIV16  = (11 << 4),
    RCC_CFGR_HPRE_DIV64  = (12 << 4),
    RCC_CFGR_HPRE_DIV128 = (13 << 4),
    RCC_CFGR_HPRE_DIV256 = (14 << 4),
    RCC_CFGR_HPRE_DIV512 = (15 << 4),
    RCC_CFGR_HPRE_MASK   = (15 << 4),
    
    // APB1 prescaler (low speed)
    RCC_CFGR_PPRE1_DIV1  = (0 << 10),
    RCC_CFGR_PPRE1_DIV2  = (4 << 10),
    RCC_CFGR_PPRE1_DIV4  = (5 << 10),
    RCC_CFGR_PPRE1_DIV8  = (6 << 10),
    RCC_CFGR_PPRE1_DIV16 = (7 << 10),
    RCC_CFGR_PPRE1_MASK  = (7 << 10),
    
    // APB2 prescaler (high speed)
    RCC_CFGR_PPRE2_DIV1  = (0 << 13),
    RCC_CFGR_PPRE2_DIV2  = (4 << 13),
    RCC_CFGR_PPRE2_DIV4  = (5 << 13),
    RCC_CFGR_PPRE2_DIV8  = (6 << 13),
    RCC_CFGR_PPRE2_DIV16 = (7 << 13),
    RCC_CFGR_PPRE2_MASK  = (7 << 13),
    
    // Microcontroller clock output
    RCC_CFGR_MCO1_MASK   = (3 << 21),
    RCC_CFGR_MCO1PRE_MASK = (7 << 24),
    RCC_CFGR_MCO2PRE_MASK = (7 << 27),
    RCC_CFGR_MCO2_MASK   = (3 << 30)
};

// Clock interrupt register (RCC_CIR)
enum RCC_CIR_Bits {
    RCC_CIR_LSIRDYF    = (1 << 0),
    RCC_CIR_LSERDYF    = (1 << 1),
    RCC_CIR_HSIRDYF    = (1 << 2),
    RCC_CIR_HSERDYF    = (1 << 3),
    RCC_CIR_PLLRDYF    = (1 << 4),
    RCC_CIR_PLLI2SRDYF = (1 << 5),
    RCC_CIR_CSSF       = (1 << 7),
    RCC_CIR_LSIRDYIE   = (1 << 8),
    RCC_CIR_LSERDYIE   = (1 << 9),
    RCC_CIR_HSIRDYIE   = (1 << 10),
    RCC_CIR_HSERDYIE   = (1 << 11),
    RCC_CIR_PLLRDYIE   = (1 << 12),
    RCC_CIR_PLLI2SRDYIE = (1 << 13),
    RCC_CIR_LSIRDYC    = (1 << 16),
    RCC_CIR_LSERDYC    = (1 << 17),
    RCC_CIR_HSIRDYC    = (1 << 18),
    RCC_CIR_HSERDYC    = (1 << 19),
    RCC_CIR_PLLRDYC    = (1 << 20),
    RCC_CIR_PLLI2SRDYC = (1 << 21),
    RCC_CIR_CSSC       = (1 << 23)
};

// AHB1 peripheral reset register (RCC_AHB1RSTR)
enum RCC_AHB1_RSTR_Bits {
    RCC_AHB1RSTR_GPIOA_RST = (1 << 0),
    RCC_AHB1RSTR_GPIOB_RST = (1 << 1),
    RCC_AHB1RSTR_GPIOC_RST = (1 << 2),
    RCC_AHB1RSTR_GPIOD_RST = (1 << 3),
    RCC_AHB1RSTR_GPIOE_RST = (1 << 4),
    RCC_AHB1RSTR_GPIOH_RST = (1 << 7),
    RCC_AHB1RSTR_CRC_RST   = (1 << 12),
    RCC_AHB1RSTR_DMA1_RST  = (1 << 21),
    RCC_AHB1RSTR_DMA2_RST  = (1 << 22)
};

// AHB2 peripheral reset register (RCC_AHB2RSTR)
enum RCC_AHB2_RSTR_Bits {
    RCC_AHB2RSTR_OTGFS_RST = (1 << 7)
};

// APB1 peripheral reset register (RCC_APB1RSTR)
enum RCC_APB1_RSTR_Bits {
    RCC_APB1RSTR_TIM2_RST   = (1 << 0),
    RCC_APB1RSTR_TIM3_RST   = (1 << 1),
    RCC_APB1RSTR_TIM4_RST   = (1 << 2),
    RCC_APB1RSTR_TIM5_RST   = (1 << 3),
    RCC_APB1RSTR_WWDG_RST   = (1 << 11),
    RCC_APB1RSTR_SPI2_RST   = (1 << 14),
    RCC_APB1RSTR_SPI3_RST   = (1 << 15),
    RCC_APB1RSTR_USART2_RST = (1 << 17),
    RCC_APB1RSTR_I2C1_RST   = (1 << 21),
    RCC_APB1RSTR_I2C2_RST   = (1 << 22),
    RCC_APB1RSTR_I2C3_RST   = (1 << 23),
    RCC_APB1RSTR_PWR_RST    = (1 << 28)
};

// APB2 peripheral reset register (RCC_APB2RSTR)
enum RCC_APB2_RSTR_Bits {
    RCC_APB2RSTR_TIM1_RST   = (1 << 0),
    RCC_APB2RSTR_USART1_RST = (1 << 4),
    RCC_APB2RSTR_USART6_RST = (1 << 5),
    RCC_APB2RSTR_ADC_RST    = (1 << 8),
    RCC_APB2RSTR_SDIO_RST   = (1 << 11),
    RCC_APB2RSTR_SPI1_RST   = (1 << 12),
    RCC_APB2RSTR_SPI4_RST   = (1 << 13),
    RCC_APB2RSTR_SYSCFG_RST = (1 << 14),
    RCC_APB2RSTR_TIM9_RST   = (1 << 16),
    RCC_APB2RSTR_TIM10_RST  = (1 << 17),
    RCC_APB2RSTR_TIM11_RST  = (1 << 18)
};
