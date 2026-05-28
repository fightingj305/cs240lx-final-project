#include "adc.h"
#include "rcc.h"
#include "utils.h"

void ADC_Init() {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1_EN;

    ADC1->CR1 = ADC_CR1_RES_12BIT;
    ADC1->CR2 = ADC_CR2_ADON;
    ADC1->SQR1 = 1 << ADC_SQR1_L_BIT; // we will only allow one conversion at a time
}


void ADC_Config_Pin(ADC_Pin *adc_pin) {
    ASSERT(adc_pin->channel <= ADC1_CH9);
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP(adc_pin->channel, ADC_SMPR_MASK);
    ADC1->SMPR2 |= ADC_SMPR2_SMP(adc_pin->channel, ADC_SMPR_3_CYCLES);
    Pin_Config(adc_pin->pin, PIN_MODE_ANALOG, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
}

uint16_t ADC_Read_Pin(ADC_Pin *adc_pin) {
    ASSERT(adc_pin->channel <= ADC1_CH9);
    ADC1->SQR3 = ADC_SQR3_SQ(1, adc_pin->channel);
    ADC1->CR2 |= ADC_CR2_SWSTART;

    while (!(ADC1->SR & ADC_SR_EOC));
    return (uint16_t)ADC1->DR;
}
