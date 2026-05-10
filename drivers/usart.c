#include "usart.h"
#include "rcc.h"
#include "utils.h"

void USART_Init(USART *usart) {
    switch ((uint32_t)usart->periph) {
        case (uint32_t)USART1:
            Pin_Config_AF(usart->tx, PIN_AF7, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(usart->rx, PIN_AF7, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            RCC->APB2ENR |= RCC_APB2ENR_USART1_EN;
            break;
        case (uint32_t)USART2:
            Pin_Config_AF(usart->tx, PIN_AF7, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(usart->rx, PIN_AF7, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            RCC->APB1ENR |= RCC_APB1ENR_USART2_EN;
            break;
        case (uint32_t)USART6:
            Pin_Config_AF(usart->tx, PIN_AF8, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(usart->rx, PIN_AF8, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            RCC->APB2ENR |= RCC_APB2ENR_USART6_EN;
            break;
        default:
            PANIC("Invalid USART peripheral");
    }
    uint32_t baud_divisor;
    switch (usart->baud_rate) {
        case USART_BAUD_RATE_9600:
            baud_divisor = usart->periph == USART2 ? (273 << USART_BRR_DIV_MANTISSA_BIT) | 7 : (546 << USART_BRR_DIV_MANTISSA_BIT) | 14;
            break;
        case USART_BAUD_RATE_19200:
            baud_divisor = usart->periph == USART2 ? (136 << USART_BRR_DIV_MANTISSA_BIT) | 12 : (273 << USART_BRR_DIV_MANTISSA_BIT) | 7;
            break;
        case USART_BAUD_RATE_38400:
            baud_divisor = usart->periph == USART2 ? (68 << USART_BRR_DIV_MANTISSA_BIT) | 6 : (136 << USART_BRR_DIV_MANTISSA_BIT) | 12;
            break;
        case USART_BAUD_RATE_57600:
            baud_divisor = usart->periph == USART2 ? (45 << USART_BRR_DIV_MANTISSA_BIT) | 9 : (91 << USART_BRR_DIV_MANTISSA_BIT) | 2;
            break;
        case USART_BAUD_RATE_115200:
            baud_divisor = usart->periph == USART2 ? (22 << USART_BRR_DIV_MANTISSA_BIT) | 13 : (45 << USART_BRR_DIV_MANTISSA_BIT) | 9;
            break;
        default:
            PANIC("Invalid USART baud rate");
            break;
    }
    usart->periph->BRR = baud_divisor;
    usart->periph->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
    usart->periph->CR2 = USART_CR2_STOP_1;
}

void USART_Send_Byte(USART *usart, uint8_t byte) {
    while (!(usart->periph->SR & USART_SR_TXE));
    usart->periph->DR = byte;
}

uint8_t USART_Receive_Byte(USART *usart) {
    while (!(usart->periph->SR & USART_SR_RXNE));
    return usart->periph->DR & USART_DR_MASK;
}

void USART_Write(USART *usart, const uint8_t *data, uint32_t length) {
    ASSERT(data != NULL);
    for (uint32_t i = 0; i < length; i++) {
        USART_Send_Byte(usart, data[i]);
    }
}

void USART_Read(USART *usart, uint8_t *data, uint32_t length) {
    ASSERT(data != NULL);
    for (uint32_t i = 0; i < length; i++) {
        data[i] = USART_Receive_Byte(usart);
    }
}