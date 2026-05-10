#pragma once
#include <stdint.h>
#include "pin.h"

#define TIM1 ((TIM_Regs *)0x40010000U)

typedef struct {
    volatile uint32_t CR1;     // Control register 1
    volatile uint32_t CR2;     // Control register 2
    volatile uint32_t SMCR;    // Slave mode control register
    volatile uint32_t DIER;    // DMA/Interrupt enable register
    volatile uint32_t SR;      // Status register
    volatile uint32_t EGR;     // Event generation register
    volatile uint32_t CCMR1;   // Capture/compare mode register 1
    volatile uint32_t CCMR2;   // Capture/compare mode register 2
    volatile uint32_t CCER;    // Capture/compare enable register
    volatile uint32_t CNT;     // Counter
    volatile uint32_t PSC;     // Prescaler
    volatile uint32_t ARR;     // Auto-reload register
    volatile uint32_t RCR;     // Repetition counter register
    volatile uint32_t CCR1;    // Capture/compare register 1
    volatile uint32_t CCR2;    // Capture/compare register 2
    volatile uint32_t CCR3;    // Capture/compare register 3
    volatile uint32_t CCR4;    // Capture/compare register 4
    volatile uint32_t BDTR;    // Break and dead-time register
    volatile uint32_t DCR;     // DMA control register
    volatile uint32_t DMAR;    // DMA address for full transfer
} TIM_Regs;

typedef enum {
    TIM_CHANNEL_1 = 0,
    TIM_CHANNEL_2 = 1,
    TIM_CHANNEL_3 = 2,
    TIM_CHANNEL_4 = 3
} TIM_Channel;


void TIM1_Init(uint32_t frequency);
void TIM1_Start();
void TIM1_Stop();
void TIM1_Config_PWM(TIM_Channel channel, Pin *pin);
void TIM1_Set_Duty_Cycle(TIM_Channel channel, uint8_t duty_cycle);

// Control Register 1 (TIM_CR1)
enum TIM_CR1_Bits {
    TIM_CR1_CEN      = (1 << 0),   // Counter enable
    TIM_CR1_UDIS     = (1 << 1),   // Update disable
    TIM_CR1_URS      = (1 << 2),   // Update request source
    TIM_CR1_OPM      = (1 << 3),   // One-pulse mode
    TIM_CR1_DIR      = (1 << 4),   // Direction (0=up, 1=down)
    
    // Center-aligned mode selection
    TIM_CR1_CMS_EDGE = (0 << 5),   // Edge-aligned mode
    TIM_CR1_CMS_1    = (1 << 5),   // Center-aligned mode 1
    TIM_CR1_CMS_2    = (2 << 5),   // Center-aligned mode 2
    TIM_CR1_CMS_3    = (3 << 5),   // Center-aligned mode 3
    TIM_CR1_CMS_MASK = (3 << 5),
    
    TIM_CR1_ARPE     = (1 << 7),   // Auto-reload preload enable
    
    // Clock division
    TIM_CR1_CKD_DIV1 = (0 << 8),   // tDTS = tCK_INT
    TIM_CR1_CKD_DIV2 = (1 << 8),   // tDTS = 2 × tCK_INT
    TIM_CR1_CKD_DIV4 = (2 << 8),   // tDTS = 4 × tCK_INT
    TIM_CR1_CKD_MASK = (3 << 8)
};

// Control Register 2 (TIM_CR2)
enum TIM_CR2_Bits {
    TIM_CR2_CCPC     = (1 << 0),   // Capture/compare preloaded control
    TIM_CR2_CCUS     = (1 << 2),   // Capture/compare control update selection
    TIM_CR2_CCDS     = (1 << 3),   // Capture/compare DMA selection
    
    // Master mode selection
    TIM_CR2_MMS_RESET     = (0 << 4),
    TIM_CR2_MMS_ENABLE    = (1 << 4),
    TIM_CR2_MMS_UPDATE    = (2 << 4),
    TIM_CR2_MMS_COMPARE   = (3 << 4),
    TIM_CR2_MMS_COMPARE1  = (4 << 4),
    TIM_CR2_MMS_COMPARE2  = (5 << 4),
    TIM_CR2_MMS_COMPARE3  = (6 << 4),
    TIM_CR2_MMS_COMPARE4  = (7 << 4),
    TIM_CR2_MMS_MASK      = (7 << 4),
    
    TIM_CR2_TI1S     = (1 << 7),   // TI1 selection
    TIM_CR2_OIS1     = (1 << 8),   // Output Idle state 1 (OC1 output)
    TIM_CR2_OIS1N    = (1 << 9),   // Output Idle state 1 (OC1N output)
    TIM_CR2_OIS2     = (1 << 10),  // Output Idle state 2 (OC2 output)
    TIM_CR2_OIS2N    = (1 << 11),  // Output Idle state 2 (OC2N output)
    TIM_CR2_OIS3     = (1 << 12),  // Output Idle state 3 (OC3 output)
    TIM_CR2_OIS3N    = (1 << 13),  // Output Idle state 3 (OC3N output)
    TIM_CR2_OIS4     = (1 << 14)   // Output Idle state 4 (OC4 output)
};

// Slave Mode Control Register (TIM_SMCR)
enum TIM_SMCR_Bits {
    // Slave mode selection
    TIM_SMCR_SMS_DISABLED   = (0 << 0),
    TIM_SMCR_SMS_ENCODER1   = (1 << 0),
    TIM_SMCR_SMS_ENCODER2   = (2 << 0),
    TIM_SMCR_SMS_ENCODER3   = (3 << 0),
    TIM_SMCR_SMS_RESET      = (4 << 0),
    TIM_SMCR_SMS_GATED      = (5 << 0),
    TIM_SMCR_SMS_TRIGGER    = (6 << 0),
    TIM_SMCR_SMS_EXTERNAL   = (7 << 0),
    TIM_SMCR_SMS_MASK       = (7 << 0),
    
    // Trigger selection
    TIM_SMCR_TS_ITR0   = (0 << 4),
    TIM_SMCR_TS_ITR1   = (1 << 4),
    TIM_SMCR_TS_ITR2   = (2 << 4),
    TIM_SMCR_TS_ITR3   = (3 << 4),
    TIM_SMCR_TS_TI1F_ED = (4 << 4),
    TIM_SMCR_TS_TI1FP1  = (5 << 4),
    TIM_SMCR_TS_TI2FP2  = (6 << 4),
    TIM_SMCR_TS_ETRF    = (7 << 4),
    TIM_SMCR_TS_MASK    = (7 << 4),
    
    TIM_SMCR_MSM     = (1 << 7),   // Master/slave mode
    
    // External trigger filter
    TIM_SMCR_ETF_MASK = (0xF << 8),
    
    // External trigger prescaler
    TIM_SMCR_ETPS_OFF   = (0 << 12),
    TIM_SMCR_ETPS_DIV2  = (1 << 12),
    TIM_SMCR_ETPS_DIV4  = (2 << 12),
    TIM_SMCR_ETPS_DIV8  = (3 << 12),
    TIM_SMCR_ETPS_MASK  = (3 << 12),
    
    TIM_SMCR_ECE     = (1 << 14),  // External clock enable
    TIM_SMCR_ETP     = (1 << 15)   // External trigger polarity
};

// DMA/Interrupt Enable Register (TIM_DIER)
enum TIM_DIER_Bits {
    TIM_DIER_UIE     = (1 << 0),   // Update interrupt enable
    TIM_DIER_CC1IE   = (1 << 1),   // Capture/Compare 1 interrupt enable
    TIM_DIER_CC2IE   = (1 << 2),   // Capture/Compare 2 interrupt enable
    TIM_DIER_CC3IE   = (1 << 3),   // Capture/Compare 3 interrupt enable
    TIM_DIER_CC4IE   = (1 << 4),   // Capture/Compare 4 interrupt enable
    TIM_DIER_COMIE   = (1 << 5),   // COM interrupt enable
    TIM_DIER_TIE     = (1 << 6),   // Trigger interrupt enable
    TIM_DIER_BIE     = (1 << 7),   // Break interrupt enable
    TIM_DIER_UDE     = (1 << 8),   // Update DMA request enable
    TIM_DIER_CC1DE   = (1 << 9),   // Capture/Compare 1 DMA request enable
    TIM_DIER_CC2DE   = (1 << 10),  // Capture/Compare 2 DMA request enable
    TIM_DIER_CC3DE   = (1 << 11),  // Capture/Compare 3 DMA request enable
    TIM_DIER_CC4DE   = (1 << 12),  // Capture/Compare 4 DMA request enable
    TIM_DIER_COMDE   = (1 << 13),  // COM DMA request enable
    TIM_DIER_TDE     = (1 << 14)   // Trigger DMA request enable
};

// Status Register (TIM_SR)
enum TIM_SR_Bits {
    TIM_SR_UIF   = (1 << 0),   // Update interrupt flag
    TIM_SR_CC1IF = (1 << 1),   // Capture/Compare 1 interrupt flag
    TIM_SR_CC2IF = (1 << 2),   // Capture/Compare 2 interrupt flag
    TIM_SR_CC3IF = (1 << 3),   // Capture/Compare 3 interrupt flag
    TIM_SR_CC4IF = (1 << 4),   // Capture/Compare 4 interrupt flag
    TIM_SR_COMIF = (1 << 5),   // COM interrupt flag
    TIM_SR_TIF   = (1 << 6),   // Trigger interrupt flag
    TIM_SR_BIF   = (1 << 7),   // Break interrupt flag
    TIM_SR_CC1OF = (1 << 9),   // Capture/Compare 1 overcapture flag
    TIM_SR_CC2OF = (1 << 10),  // Capture/Compare 2 overcapture flag
    TIM_SR_CC3OF = (1 << 11),  // Capture/Compare 3 overcapture flag
    TIM_SR_CC4OF = (1 << 12)   // Capture/Compare 4 overcapture flag
};

// Event Generation Register (TIM_EGR)
enum TIM_EGR_Bits {
    TIM_EGR_UG   = (1 << 0),   // Update generation
    TIM_EGR_CC1G = (1 << 1),   // Capture/Compare 1 generation
    TIM_EGR_CC2G = (1 << 2),   // Capture/Compare 2 generation
    TIM_EGR_CC3G = (1 << 3),   // Capture/Compare 3 generation
    TIM_EGR_CC4G = (1 << 4),   // Capture/Compare 4 generation
    TIM_EGR_COMG = (1 << 5),   // Capture/Compare control update generation
    TIM_EGR_TG   = (1 << 6),   // Trigger generation
    TIM_EGR_BG   = (1 << 7)    // Break generation
};

// Capture/Compare Mode Register 1 (TIM_CCMR1) - Output mode
enum TIM_CCMR1_Output_Bits {
    // CC1S: Capture/Compare 1 selection
    TIM_CCMR1_CC1S_OUTPUT = (0 << 0),
    TIM_CCMR1_CC1S_INPUT_TI1 = (1 << 0),
    TIM_CCMR1_CC1S_INPUT_TI2 = (2 << 0),
    TIM_CCMR1_CC1S_INPUT_TRC = (3 << 0),
    TIM_CCMR1_CC1S_MASK = (3 << 0),
    
    TIM_CCMR1_OC1FE  = (1 << 2),   // Output compare 1 fast enable
    TIM_CCMR1_OC1PE  = (1 << 3),   // Output compare 1 preload enable
    
    // OC1M: Output compare 1 mode
    TIM_CCMR1_OC1M_FROZEN    = (0 << 4),
    TIM_CCMR1_OC1M_ACTIVE    = (1 << 4),
    TIM_CCMR1_OC1M_INACTIVE  = (2 << 4),
    TIM_CCMR1_OC1M_TOGGLE    = (3 << 4),
    TIM_CCMR1_OC1M_FORCE_LOW = (4 << 4),
    TIM_CCMR1_OC1M_FORCE_HIGH = (5 << 4),
    TIM_CCMR1_OC1M_PWM1      = (6 << 4),
    TIM_CCMR1_OC1M_PWM2      = (7 << 4),
    TIM_CCMR1_OC1M_MASK      = (7 << 4),
    
    TIM_CCMR1_OC1CE  = (1 << 7),   // Output compare 1 clear enable
    
    // CC2S: Capture/Compare 2 selection
    TIM_CCMR1_CC2S_OUTPUT = (0 << 8),
    TIM_CCMR1_CC2S_INPUT_TI2 = (1 << 8),
    TIM_CCMR1_CC2S_INPUT_TI1 = (2 << 8),
    TIM_CCMR1_CC2S_INPUT_TRC = (3 << 8),
    TIM_CCMR1_CC2S_MASK = (3 << 8),
    
    TIM_CCMR1_OC2FE  = (1 << 10),  // Output compare 2 fast enable
    TIM_CCMR1_OC2PE  = (1 << 11),  // Output compare 2 preload
    // OC2M: Output compare 2 mode
    TIM_CCMR1_OC2M_FROZEN    = (0 << 12),
    TIM_CCMR1_OC2M_ACTIVE    = (1 << 12),
    TIM_CCMR1_OC2M_INACTIVE  = (2 << 12),
    TIM_CCMR1_OC2M_TOGGLE    = (3 << 12),
    TIM_CCMR1_OC2M_FORCE_LOW = (4 << 12),
    TIM_CCMR1_OC2M_FORCE_HIGH = (5 << 12),
    TIM_CCMR1_OC2M_PWM1      = (6 << 12),
    TIM_CCMR1_OC2M_PWM2      = (7 << 12),
    TIM_CCMR1_OC2M_MASK      = (7 << 12),
    
    TIM_CCMR1_OC2CE  = (1 << 15)   // Output compare 2 clear enable
};

// Capture/Compare Mode Register 1 (TIM_CCMR1) - Input mode
enum TIM_CCMR1_Input_Bits {
    // IC1PSC: Input capture 1 prescaler
    TIM_CCMR1_IC1PSC_DIV1 = (0 << 2),
    TIM_CCMR1_IC1PSC_DIV2 = (1 << 2),
    TIM_CCMR1_IC1PSC_DIV4 = (2 << 2),
    TIM_CCMR1_IC1PSC_DIV8 = (3 << 2),
    TIM_CCMR1_IC1PSC_MASK = (3 << 2),
    
    // IC1F: Input capture 1 filter
    TIM_CCMR1_IC1F_MASK = (0xF << 4),
    
    // IC2PSC: Input capture 2 prescaler
    TIM_CCMR1_IC2PSC_DIV1 = (0 << 10),
    TIM_CCMR1_IC2PSC_DIV2 = (1 << 10),
    TIM_CCMR1_IC2PSC_DIV4 = (2 << 10),
    TIM_CCMR1_IC2PSC_DIV8 = (3 << 10),
    TIM_CCMR1_IC2PSC_MASK = (3 << 10),
    
    // IC2F: Input capture 2 filter
    TIM_CCMR1_IC2F_MASK = (0xF << 12)
};

// Capture/Compare Mode Register 2 (TIM_CCMR2) - Output mode
enum TIM_CCMR2_Output_Bits {
    // CC3S: Capture/Compare 3 selection
    TIM_CCMR2_CC3S_OUTPUT = (0 << 0),
    TIM_CCMR2_CC3S_INPUT_TI3 = (1 << 0),
    TIM_CCMR2_CC3S_INPUT_TI4 = (2 << 0),
    TIM_CCMR2_CC3S_INPUT_TRC = (3 << 0),
    TIM_CCMR2_CC3S_MASK = (3 << 0),
    
    TIM_CCMR2_OC3FE  = (1 << 2),   // Output compare 3 fast enable
    TIM_CCMR2_OC3PE  = (1 << 3),   // Output compare 3 preload enable
    
    // OC3M: Output compare 3 mode
    TIM_CCMR2_OC3M_FROZEN    = (0 << 4),
    TIM_CCMR2_OC3M_ACTIVE    = (1 << 4),
    TIM_CCMR2_OC3M_INACTIVE  = (2 << 4),
    TIM_CCMR2_OC3M_TOGGLE    = (3 << 4),
    TIM_CCMR2_OC3M_FORCE_LOW = (4 << 4),
    TIM_CCMR2_OC3M_FORCE_HIGH = (5 << 4),
    TIM_CCMR2_OC3M_PWM1      = (6 << 4),
    TIM_CCMR2_OC3M_PWM2      = (7 << 4),
    TIM_CCMR2_OC3M_MASK      = (7 << 4),
    
    TIM_CCMR2_OC3CE  = (1 << 7),   // Output compare 3 clear enable
    
    // CC4S: Capture/Compare 4 selection
    TIM_CCMR2_CC4S_OUTPUT = (0 << 8),
    TIM_CCMR2_CC4S_INPUT_TI4 = (1 << 8),
    TIM_CCMR2_CC4S_INPUT_TI3 = (2 << 8),
    TIM_CCMR2_CC4S_INPUT_TRC = (3 << 8),
    TIM_CCMR2_CC4S_MASK = (3 << 8),
    
    TIM_CCMR2_OC4FE  = (1 << 10),  // Output compare 4 fast enable
    TIM_CCMR2_OC4PE  = (1 << 11),  // Output compare 4 preload enable
    
    // OC4M: Output compare 4 mode
    TIM_CCMR2_OC4M_FROZEN    = (0 << 12),
    TIM_CCMR2_OC4M_ACTIVE    = (1 << 12),
    TIM_CCMR2_OC4M_INACTIVE  = (2 << 12),
    TIM_CCMR2_OC4M_TOGGLE    = (3 << 12),
    TIM_CCMR2_OC4M_FORCE_LOW = (4 << 12),
    TIM_CCMR2_OC4M_FORCE_HIGH = (5 << 12),
    TIM_CCMR2_OC4M_PWM1      = (6 << 12),
    TIM_CCMR2_OC4M_PWM2      = (7 << 12),
    TIM_CCMR2_OC4M_MASK      = (7 << 12),
    
    TIM_CCMR2_OC4CE  = (1 << 15)   // Output compare 4 clear enable
};

// Capture/Compare Mode Register 2 (TIM_CCMR2) - Input mode
enum TIM_CCMR2_Input_Bits {
    // IC3PSC: Input capture 3 prescaler
    TIM_CCMR2_IC3PSC_DIV1 = (0 << 2),
    TIM_CCMR2_IC3PSC_DIV2 = (1 << 2),
    TIM_CCMR2_IC3PSC_DIV4 = (2 << 2),
    TIM_CCMR2_IC3PSC_DIV8 = (3 << 2),
    TIM_CCMR2_IC3PSC_MASK = (3 << 2),
    
    // IC3F: Input capture 3 filter
    TIM_CCMR2_IC3F_MASK = (0xF << 4),
    
    // IC4PSC: Input capture 4 prescaler
    TIM_CCMR2_IC4PSC_DIV1 = (0 << 10),
    TIM_CCMR2_IC4PSC_DIV2 = (1 << 10),
    TIM_CCMR2_IC4PSC_DIV4 = (2 << 10),
    TIM_CCMR2_IC4PSC_DIV8 = (3 << 10),
    TIM_CCMR2_IC4PSC_MASK = (3 << 10),
    
    // IC4F: Input capture 4 filter
    TIM_CCMR2_IC4F_MASK = (0xF << 12)
};

// Capture/Compare Enable Register (TIM_CCER)
enum TIM_CCER_Bits {
    TIM_CCER_CC1E  = (1 << 0),   // Capture/Compare 1 output enable
    TIM_CCER_CC1P  = (1 << 1),   // Capture/Compare 1 output polarity
    TIM_CCER_CC1NE = (1 << 2),   // Capture/Compare 1 complementary output enable
    TIM_CCER_CC1NP = (1 << 3),   // Capture/Compare 1 complementary output polarity
    
    TIM_CCER_CC2E  = (1 << 4),   // Capture/Compare 2 output enable
    TIM_CCER_CC2P  = (1 << 5),   // Capture/Compare 2 output polarity
    TIM_CCER_CC2NE = (1 << 6),   // Capture/Compare 2 complementary output enable
    TIM_CCER_CC2NP = (1 << 7),   // Capture/Compare 2 complementary output polarity
    
    TIM_CCER_CC3E  = (1 << 8),   // Capture/Compare 3 output enable
    TIM_CCER_CC3P  = (1 << 9),   // Capture/Compare 3 output polarity
    TIM_CCER_CC3NE = (1 << 10),  // Capture/Compare 3 complementary output enable
    TIM_CCER_CC3NP = (1 << 11),  // Capture/Compare 3 complementary output polarity
    
    TIM_CCER_CC4E  = (1 << 12),  // Capture/Compare 4 output enable
    TIM_CCER_CC4P  = (1 << 13)   // Capture/Compare 4 output polarity
};

// Break and Dead-time Register (TIM_BDTR)
enum TIM_BDTR_Bits {
    // DTG: Dead-time generator setup
    TIM_BDTR_DTG_MASK = (0xFF << 0),
    
    // LOCK: Lock configuration
    TIM_BDTR_LOCK_OFF   = (0 << 8),
    TIM_BDTR_LOCK_LEVEL1 = (1 << 8),
    TIM_BDTR_LOCK_LEVEL2 = (2 << 8),
    TIM_BDTR_LOCK_LEVEL3 = (3 << 8),
    TIM_BDTR_LOCK_MASK   = (3 << 8),
    
    TIM_BDTR_OSSI = (1 << 10),  // Off-state selection for idle mode
    TIM_BDTR_OSSR = (1 << 11),  // Off-state selection for run mode
    TIM_BDTR_BKE  = (1 << 12),  // Break enable
    TIM_BDTR_BKP  = (1 << 13),  // Break polarity
    TIM_BDTR_AOE  = (1 << 14),  // Automatic output enable
    TIM_BDTR_MOE  = (1 << 15)   // Main output enable
};

// DMA Control Register (TIM_DCR)
enum TIM_DCR_Bits {
    // DBA: DMA base address
    TIM_DCR_DBA_MASK = (0x1F << 0),
    
    // DBL: DMA burst length
    TIM_DCR_DBL_1   = (0 << 8),
    TIM_DCR_DBL_2   = (1 << 8),
    TIM_DCR_DBL_3   = (2 << 8),
    TIM_DCR_DBL_4   = (3 << 8),
    TIM_DCR_DBL_5   = (4 << 8),
    TIM_DCR_DBL_6   = (5 << 8),
    TIM_DCR_DBL_7   = (6 << 8),
    TIM_DCR_DBL_8   = (7 << 8),
    TIM_DCR_DBL_9   = (8 << 8),
    TIM_DCR_DBL_10  = (9 << 8),
    TIM_DCR_DBL_11  = (10 << 8),
    TIM_DCR_DBL_12  = (11 << 8),
    TIM_DCR_DBL_13  = (12 << 8),
    TIM_DCR_DBL_14  = (13 << 8),
    TIM_DCR_DBL_15  = (14 << 8),
    TIM_DCR_DBL_16  = (15 << 8),
    TIM_DCR_DBL_17  = (16 << 8),
    TIM_DCR_DBL_18  = (17 << 8),
    TIM_DCR_DBL_MASK = (0x1F << 8)
};
