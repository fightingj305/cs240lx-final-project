#include "timer.h"
#include "rcc.h"
#include "utils.h"

#define TIM1_ARR 1000

void TIM1_Init(uint32_t frequency) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1_EN;

    TIM1->PSC = (APB2_CLK / frequency) - 1;
    TIM1->ARR = TIM1_ARR - 1; 
    TIM1->CR1 = TIM_CR1_ARPE;
    TIM1->EGR = TIM_EGR_UG;
    TIM1->BDTR = TIM_BDTR_MOE;
    TIM1_Start();
}

void TIM1_Start() {
    TIM1->CR1 |= TIM_CR1_CEN;
}

void TIM1_Stop() {
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

void TIM1_Config_PWM(TIM_Channel channel, Pin *pin) {
    switch (channel) {
        case TIM_CHANNEL_1:
            ASSERT(pin->pin_num == 8 && pin->port == GPIOA);
            TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_MASK;
            TIM1->CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_CC1S_OUTPUT;
            TIM1->CCR1 = 0;
            TIM1->CCER |= TIM_CCER_CC1E; 
            break;
        
        case TIM_CHANNEL_2:
            ASSERT(pin->pin_num == 9 && pin->port == GPIOA);
            TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_MASK;
            TIM1->CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_CC2S_OUTPUT;
            TIM1->CCR2 = 0;
            TIM1->CCER |= TIM_CCER_CC2E; 
            break;

        case TIM_CHANNEL_3:
            ASSERT(pin->pin_num == 10 && pin->port == GPIOA);
            TIM1->CCMR2 &= ~TIM_CCMR2_OC3M_MASK;
            TIM1->CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_CC3S_OUTPUT;
            TIM1->CCR3 = 0;
            TIM1->CCER |= TIM_CCER_CC3E;
            break;

        case TIM_CHANNEL_4:
            ASSERT(pin->pin_num == 11 && pin->port == GPIOA);
            TIM1->CCMR2 &= ~TIM_CCMR2_OC4M_MASK;
            TIM1->CCMR2 |= TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_CC4S_OUTPUT;
            TIM1->CCR4 = 0;
            TIM1->CCER |= TIM_CCER_CC4E;
            break;
        default:
            break;
    }
    Pin_Config_AF(pin, PIN_AF1, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_NONE);
}

void TIM1_Set_Duty_Cycle(TIM_Channel channel, uint8_t duty_cycle) {
    ASSERT(duty_cycle <= 100);
    uint32_t ccr_value = (TIM1_ARR / 100) * duty_cycle;
    switch (channel) {
        case TIM_CHANNEL_1:
            TIM1->CCR1 = ccr_value;
            break;
        case TIM_CHANNEL_2:
            TIM1->CCR2 = ccr_value;
            break;
        case TIM_CHANNEL_3:
            TIM1->CCR3 = ccr_value;
            break;
        case TIM_CHANNEL_4:
            TIM1->CCR4 = ccr_value;
            break;
        default:
            break;
    }
}