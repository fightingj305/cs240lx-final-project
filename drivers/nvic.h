#pragma once
#include <stdint.h>

#define NVIC ((NVIC_Regs *)0xE000E100U)

typedef struct {
    volatile uint32_t ISER[8];
    uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];
    uint32_t RESERVED1[24];
    volatile uint32_t ISPR[8];
    uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];
    uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];
    uint32_t RESERVED4[56];
    volatile uint8_t  IP[240];
    uint32_t RESERVED5[644];
    volatile uint32_t STIR;
} NVIC_Regs;

typedef enum {
    NonMaskableInt_IRQn         = -14,
    HardFault_IRQn              = -13,
    MemoryManagement_IRQn       = -12,
    BusFault_IRQn               = -11,
    UsageFault_IRQn             = -10,
    SVCall_IRQn                 = -5,
    DebugMonitor_IRQn           = -4,
    PendSV_IRQn                 = -2,
    SysTick_IRQn                = -1,
    
    WWDG_IRQn                   = 0,
    PVD_IRQn                    = 1,
    TAMP_STAMP_IRQn             = 2,
    RTC_WKUP_IRQn               = 3,
    FLASH_IRQn                  = 4,
    RCC_IRQn                    = 5,
    EXTI0_IRQn                  = 6,
    EXTI1_IRQn                  = 7,
    EXTI2_IRQn                  = 8,
    EXTI3_IRQn                  = 9,
    EXTI4_IRQn                  = 10,
    DMA1_Stream0_IRQn           = 11,
    DMA1_Stream1_IRQn           = 12,
    DMA1_Stream2_IRQn           = 13,
    DMA1_Stream3_IRQn           = 14,
    DMA1_Stream4_IRQn           = 15,
    DMA1_Stream5_IRQn           = 16,
    DMA1_Stream6_IRQn           = 17,
    ADC_IRQn                    = 18,
    EXTI9_5_IRQn                = 23,
    TIM1_BRK_TIM9_IRQn          = 24,
    TIM1_UP_TIM10_IRQn          = 25,
    TIM1_TRG_COM_TIM11_IRQn     = 26,
    TIM1_CC_IRQn                = 27,
    TIM2_IRQn                   = 28,
    TIM3_IRQn                   = 29,
    TIM4_IRQn                   = 30,
    I2C1_EV_IRQn                = 31,
    I2C1_ER_IRQn                = 32,
    I2C2_EV_IRQn                = 33,
    I2C2_ER_IRQn                = 34,
    SPI1_IRQn                   = 35,
    SPI2_IRQn                   = 36,
    USART1_IRQn                 = 37,
    USART2_IRQn                 = 38,
    EXTI15_10_IRQn              = 40,
    RTC_Alarm_IRQn              = 41,
    OTG_FS_WKUP_IRQn            = 42,
    DMA1_Stream7_IRQn           = 47,
    SDIO_IRQn                   = 49,
    TIM5_IRQn                   = 50,
    SPI3_IRQn                   = 51,
    DMA2_Stream0_IRQn           = 56,
    DMA2_Stream1_IRQn           = 57,
    DMA2_Stream2_IRQn           = 58,
    DMA2_Stream3_IRQn           = 59,
    DMA2_Stream4_IRQn           = 60,
    OTG_FS_IRQn                 = 67,
    DMA2_Stream5_IRQn           = 68,
    DMA2_Stream6_IRQn           = 69,
    DMA2_Stream7_IRQn           = 70,
    USART6_IRQn                 = 71,
    I2C3_EV_IRQn                = 72,
    I2C3_ER_IRQn                = 73,
    FPU_IRQn                    = 81,
    SPI4_IRQn                   = 84,
    SPI5_IRQn                   = 85
} IRQn;

enum NVIC_Priority {
    NVIC_PRIORITY_0  = 0,   // highest
    NVIC_PRIORITY_1  = 1,
    NVIC_PRIORITY_2  = 2,
    NVIC_PRIORITY_3  = 3,
    NVIC_PRIORITY_4  = 4,
    NVIC_PRIORITY_5  = 5,
    NVIC_PRIORITY_6  = 6,
    NVIC_PRIORITY_7  = 7,
    NVIC_PRIORITY_8  = 8,
    NVIC_PRIORITY_9  = 9,
    NVIC_PRIORITY_10 = 10,
    NVIC_PRIORITY_11 = 11,
    NVIC_PRIORITY_12 = 12,
    NVIC_PRIORITY_13 = 13,
    NVIC_PRIORITY_14 = 14,
    NVIC_PRIORITY_15 = 15
};

#define NVIC_EnableIRQ(IRQn)     (NVIC->ISER[(uint32_t)(IRQn) >> 5] = (1 << ((uint32_t)(IRQn) & 0x1F)))
#define NVIC_DisableIRQ(IRQn)    (NVIC->ICER[(uint32_t)(IRQn) >> 5] = (1 << ((uint32_t)(IRQn) & 0x1F)))
#define NVIC_GetPendingIRQ(IRQn) ((NVIC->ISPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F))) ? 1 : 0)
#define NVIC_SetPendingIRQ(IRQn) (NVIC->ISPR[(uint32_t)(IRQn) >> 5] = (1 << ((uint32_t)(IRQn) & 0x1F)))
#define NVIC_ClearPendingIRQ(IRQn) (NVIC->ICPR[(uint32_t)(IRQn) >> 5] = (1 << ((uint32_t)(IRQn) & 0x1F)))
#define NVIC_GetActive(IRQn)     ((NVIC->IABR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F))) ? 1 : 0)

// only top 4 bits used
#define NVIC_SetPriority(IRQn, priority) (NVIC->IP[(uint32_t)(IRQn)] = ((priority) << 4))
#define NVIC_GetPriority(IRQn)           (NVIC->IP[(uint32_t)(IRQn)] >> 4)