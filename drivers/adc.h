#pragma once
// Lowkey very inefficient ADC driver; good enough for the base station


#include <stdint.h>
#include "pin.h"

#define ADC1 ((ADC_Regs *)0x40012000U)
#define ADC_COMMON ((ADC_Common_Regs *)0x40012300U)

typedef struct {
    volatile uint32_t SR;      // Status register
    volatile uint32_t CR1;     // Control register 1
    volatile uint32_t CR2;     // Control register 2
    volatile uint32_t SMPR1;   // Sample time register 1
    volatile uint32_t SMPR2;   // Sample time register 2
    volatile uint32_t JOFR1;   // Injected channel data offset register 1
    volatile uint32_t JOFR2;   // Injected channel data offset register 2
    volatile uint32_t JOFR3;   // Injected channel data offset register 3
    volatile uint32_t JOFR4;   // Injected channel data offset register 4
    volatile uint32_t HTR;     // Watchdog high threshold register
    volatile uint32_t LTR;     // Watchdog low threshold register
    volatile uint32_t SQR1;    // Regular sequence register 1
    volatile uint32_t SQR2;    // Regular sequence register 2
    volatile uint32_t SQR3;    // Regular sequence register 3
    volatile uint32_t JSQR;    // Injected sequence register
    volatile uint32_t JDR1;    // Injected data register 1
    volatile uint32_t JDR2;    // Injected data register 2
    volatile uint32_t JDR3;    // Injected data register 3
    volatile uint32_t JDR4;    // Injected data register 4
    volatile uint32_t DR;      // Regular data register
} ADC_Regs;

typedef enum {
    ADC1_CH0,
    ADC1_CH1,
    ADC1_CH2,
    ADC1_CH3,
    ADC1_CH4,
    ADC1_CH5,
    ADC1_CH6,
    ADC1_CH7,
    ADC1_CH8,
    ADC1_CH9
} ADC_Channel;

typedef struct 
{
    Pin *pin;
    ADC_Channel channel;
} ADC_Pin;


void ADC_Init();
void ADC_Config_Pin(ADC_Pin *adc_pin);
uint16_t ADC_Read_Pin(ADC_Pin *adc_pin); // blocking single conversion
typedef struct {
    volatile uint32_t reserved; 
    volatile uint32_t CCR;     // Common control register
} ADC_Common_Regs;

// Status Register (ADC_SR)
enum ADC_SR_Bits {
    ADC_SR_OVR      = (1 << 5),  // Overrun
    ADC_SR_STRT     = (1 << 4),  // Regular channel start flag
    ADC_SR_JSTRT    = (1 << 3),  // Injected channel start flag
    ADC_SR_JEOC     = (1 << 2),  // Injected channel end of conversion
    ADC_SR_EOC      = (1 << 1),  // Regular channel end of conversion
    ADC_SR_AWD      = (1 << 0)   // Analog watchdog flag
};

// Control Register 1 (ADC_CR1)
enum ADC_CR1_Bits {
    ADC_CR1_OVRIE   = (1 << 26), // Overrun interrupt enable
    
    // Resolution
    ADC_CR1_RES_12BIT = (0 << 24),
    ADC_CR1_RES_10BIT = (1 << 24),
    ADC_CR1_RES_8BIT  = (2 << 24),
    ADC_CR1_RES_6BIT  = (3 << 24),
    ADC_CR1_RES_MASK  = (3 << 24),
    
    ADC_CR1_AWDEN   = (1 << 23), // Analog watchdog enable on regular channels
    ADC_CR1_JAWDEN  = (1 << 22), // Analog watchdog enable on injected channels
    
    // Discontinuous mode channel count
    ADC_CR1_DISCNUM_BIT  = 13,
    ADC_CR1_DISCNUM_MASK = (7 << 13),
    
    ADC_CR1_JDISCEN = (1 << 12), // Discontinuous mode on injected channels
    ADC_CR1_DISCEN  = (1 << 11), // Discontinuous mode on regular channels
    ADC_CR1_JAUTO   = (1 << 10), // Automatic injected group conversion
    ADC_CR1_AWDSGL  = (1 << 9),  // Enable watchdog on single channel
    ADC_CR1_SCAN    = (1 << 8),  // Scan mode
    ADC_CR1_JEOCIE  = (1 << 7),  // Interrupt enable for injected channels
    ADC_CR1_AWDIE   = (1 << 6),  // Analog watchdog interrupt enable
    ADC_CR1_EOCIE   = (1 << 5),  // Interrupt enable for EOC
    
    // Analog watchdog channel select
    ADC_CR1_AWDCH_BIT  = 0,
    ADC_CR1_AWDCH_MASK = (0x1F << 0)
};

// Control Register 2 (ADC_CR2)
enum ADC_CR2_Bits {
    ADC_CR2_SWSTART = (1 << 30), // Start conversion of regular channels
    
    // External trigger enable for regular channels
    ADC_CR2_EXTEN_DISABLED     = (0 << 28),
    ADC_CR2_EXTEN_RISING       = (1 << 28),
    ADC_CR2_EXTEN_FALLING      = (2 << 28),
    ADC_CR2_EXTEN_BOTH         = (3 << 28),
    ADC_CR2_EXTEN_MASK         = (3 << 28),
    
    // External event select for regular group
    ADC_CR2_EXTSEL_TIM1_CC1    = (0 << 24),
    ADC_CR2_EXTSEL_TIM1_CC2    = (1 << 24),
    ADC_CR2_EXTSEL_TIM1_CC3    = (2 << 24),
    ADC_CR2_EXTSEL_TIM2_CC2    = (3 << 24),
    ADC_CR2_EXTSEL_TIM2_CC3    = (4 << 24),
    ADC_CR2_EXTSEL_TIM2_CC4    = (5 << 24),
    ADC_CR2_EXTSEL_TIM2_TRGO   = (6 << 24),
    ADC_CR2_EXTSEL_TIM3_CC1    = (7 << 24),
    ADC_CR2_EXTSEL_TIM3_TRGO   = (8 << 24),
    ADC_CR2_EXTSEL_TIM4_CC4    = (9 << 24),
    ADC_CR2_EXTSEL_TIM5_CC1    = (10 << 24),
    ADC_CR2_EXTSEL_TIM5_CC2    = (11 << 24),
    ADC_CR2_EXTSEL_TIM5_CC3    = (12 << 24),
    ADC_CR2_EXTSEL_EXTI_LINE11 = (15 << 24),
    ADC_CR2_EXTSEL_MASK        = (0xF << 24),
    
    ADC_CR2_JSWSTART = (1 << 22), // Start conversion of injected channels
    
    // External trigger enable for injected channels
    ADC_CR2_JEXTEN_DISABLED    = (0 << 20),
    ADC_CR2_JEXTEN_RISING      = (1 << 20),
    ADC_CR2_JEXTEN_FALLING     = (2 << 20),
    ADC_CR2_JEXTEN_BOTH        = (3 << 20),
    ADC_CR2_JEXTEN_MASK        = (3 << 20),
    
    // External event select for injected group
    ADC_CR2_JEXTSEL_TIM1_CC4   = (0 << 16),
    ADC_CR2_JEXTSEL_TIM1_TRGO  = (1 << 16),
    ADC_CR2_JEXTSEL_TIM2_CC1   = (2 << 16),
    ADC_CR2_JEXTSEL_TIM2_TRGO  = (3 << 16),
    ADC_CR2_JEXTSEL_TIM3_CC2   = (4 << 16),
    ADC_CR2_JEXTSEL_TIM3_CC4   = (5 << 16),
    ADC_CR2_JEXTSEL_TIM4_CC1   = (6 << 16),
    ADC_CR2_JEXTSEL_TIM4_CC2   = (7 << 16),
    ADC_CR2_JEXTSEL_TIM4_CC3   = (8 << 16),
    ADC_CR2_JEXTSEL_TIM4_TRGO  = (9 << 16),
    ADC_CR2_JEXTSEL_TIM5_CC4   = (10 << 16),
    ADC_CR2_JEXTSEL_TIM5_TRGO  = (11 << 16),
    ADC_CR2_JEXTSEL_EXTI_LINE15 = (15 << 16),
    ADC_CR2_JEXTSEL_MASK       = (0xF << 16),
    
    ADC_CR2_ALIGN   = (1 << 11), // Data alignment (0=right, 1=left)
    ADC_CR2_EOCS    = (1 << 10), // End of conversion selection
    ADC_CR2_DDS     = (1 << 9),  // DMA disable selection
    ADC_CR2_DMA     = (1 << 8),  // Direct memory access mode
    ADC_CR2_CONT    = (1 << 1),  // Continuous conversion
    ADC_CR2_ADON    = (1 << 0)   // A/D converter ON/OFF
};

// Sample Time Register 1 (ADC_SMPR1) - Channels 10-18
// Sample Time Register 2 (ADC_SMPR2) - Channels 0-9
enum ADC_SMPR_Bits {
    ADC_SMPR_3_CYCLES   = 0,
    ADC_SMPR_15_CYCLES  = 1,
    ADC_SMPR_28_CYCLES  = 2,
    ADC_SMPR_56_CYCLES  = 3,
    ADC_SMPR_84_CYCLES  = 4,
    ADC_SMPR_112_CYCLES = 5,
    ADC_SMPR_144_CYCLES = 6,
    ADC_SMPR_480_CYCLES = 7,
    ADC_SMPR_MASK        = 0x7
};

// Helper macros for sample time configuration
#define ADC_SMPR1_SMP(channel, cycles) (((cycles) & 0x7) << (((channel) - 10) * 3))
#define ADC_SMPR2_SMP(channel, cycles) (((cycles) & 0x7) << ((channel) * 3))

// Regular Sequence Registers
// SQR1: Sequence length and channels 13-16
// SQR2: Channels 7-12
// SQR3: Channels 1-6
enum ADC_SQR_Bits {
    ADC_SQR1_L_BIT  = 20,  // Regular channel sequence length (0-15 = 1-16 conversions)
    ADC_SQR1_L_MASK = (0xF << 20)
};

#define ADC_SQR1_SQ(n, channel) (((channel) & 0x1F) << (((n) - 13) * 5))  // n = 13-16
#define ADC_SQR2_SQ(n, channel) (((channel) & 0x1F) << (((n) - 7) * 5))   // n = 7-12
#define ADC_SQR3_SQ(n, channel) (((channel) & 0x1F) << (((n) - 1) * 5))   // n = 1-6

// Injected Sequence Register (ADC_JSQR)
enum ADC_JSQR_Bits {
    ADC_JSQR_JL_BIT  = 20,  // Injected sequence length (0-3 = 1-4 conversions)
    ADC_JSQR_JL_MASK = (0x3 << 20)
};

#define ADC_JSQR_JSQ(n, channel) (((channel) & 0x1F) << (((n) - 1) * 5))  // n = 1-4

// Common Control Register (ADC_CCR)
enum ADC_CCR_Bits {
    ADC_CCR_TSVREFE = (1 << 23), // Temperature sensor and VREFINT enable
    ADC_CCR_VBATE   = (1 << 22), // VBAT enable
    
    // ADC prescaler
    ADC_CCR_ADCPRE_DIV2 = (0 << 16),
    ADC_CCR_ADCPRE_DIV4 = (1 << 16),
    ADC_CCR_ADCPRE_DIV6 = (2 << 16),
    ADC_CCR_ADCPRE_DIV8 = (3 << 16),
    ADC_CCR_ADCPRE_MASK = (3 << 16),
};