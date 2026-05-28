#include "exti.h"
#include "rcc.h"
#include "nvic.h"

void EXTI_Config(Pin *pin, EXTI_Trigger trigger) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFG_EN;
    SYSCFG_EXTICR_Set(pin);
    EXTI->IMR |= (1 << pin->pin_num);
    
    if (trigger & EXTI_TRIGGER_RISING) {
        EXTI->RTSR |= (1 << pin->pin_num);
    } else {
        EXTI->RTSR &= ~(1 << pin->pin_num);
    }
    
    if (trigger & EXTI_TRIGGER_FALLING) {
        EXTI->FTSR |= (1 << pin->pin_num);
    } else {
        EXTI->FTSR &= ~(1 << pin->pin_num);
    }
    
    if (pin->pin_num < 5) {
        NVIC_EnableIRQ(EXTI0_IRQn + pin->pin_num);
    } else if (pin->pin_num >= 5 && pin->pin_num <= 9) {
        NVIC_EnableIRQ(EXTI9_5_IRQn);
    } else if (pin->pin_num >= 10 && pin->pin_num <= 15) {
        NVIC_EnableIRQ(EXTI15_10_IRQn);
    }
}
