#include "i2c.h"
#include "rcc.h"
#include "utils.h"

void I2C_Init(I2C *i2c) {
    switch ((uint32_t) i2c->periph) {
        case (uint32_t) I2C1:
            RCC->APB1ENR |= RCC_APB1ENR_I2C1_EN;
            break;
        case (uint32_t) I2C2:
            RCC->APB1ENR |= RCC_APB1ENR_I2C2_EN;
            break;
        case (uint32_t) I2C3:
            RCC->APB1ENR |= RCC_APB1ENR_I2C3_EN;
            break;
        default:
            PANIC("Invalid I2C peripheral!");
            break;
    }
    Pin_Config_AF(i2c->scl, PIN_AF4, PIN_OT_OPEN_DRAIN, PIN_SPEED_HIGH, PIN_PULL_UP);
    Pin_Config_AF(i2c->sda, PIN_AF4, PIN_OT_OPEN_DRAIN, PIN_SPEED_HIGH, PIN_PULL_UP);

    i2c->periph->CR2 = APB1_CLK / 1000000;
    if (i2c->speed == I2C_SPEED_STANDARD) {
        i2c->periph->CCR = (APB1_CLK / 100000) /2; // 100 kHz at 50% duty
        i2c->periph->TRISE = (APB1_CLK / 1000000) + 1; // max sm rise time is 1000ns
    }
    else {
        i2c->periph->CCR = I2C_CCR_FS | (APB1_CLK / 400000) /3; // 400 kHz with 33% duty cycle
        i2c->periph->TRISE = ((APB1_CLK / 1000000) * 300) / 1000 + 1; // max fm rise time is 300ns
    }
    i2c->periph->CR1 = I2C_CR1_ACK;
    i2c->periph->CR1 |= I2C_CR1_PE;
}
void I2C_Write(I2C *i2c, uint8_t address, const uint8_t *data, uint32_t length) {
    ASSERT(address < 0x80);
    i2c->periph->CR1 |= I2C_CR1_START;
    while (!(i2c->periph->SR1 & I2C_SR1_SB));
    i2c->periph->DR = address << 1 | I2C_WRITE_BIT;
    while (!(i2c->periph->SR1 & I2C_SR1_ADDR));
    (void)i2c->periph->SR1;
    (void)i2c->periph->SR2;
    for (uint32_t i = 0; i < length; i++) {
        while (!(i2c->periph->SR1 & I2C_SR1_TXE));
        i2c->periph->DR = data[i];
    }
    while (!(i2c->periph->SR1 & I2C_SR1_BTF));
    i2c->periph->CR1 |= I2C_CR1_STOP;
    while (i2c->periph->SR2 & I2C_SR2_MSL);
}
void I2C_Read(I2C *i2c, uint8_t address, uint8_t *data, uint32_t length) {
    ASSERT(address < 0x80);
    // Procedure on datasheet pg. 478
    i2c->periph->CR1 |= I2C_CR1_ACK;
    i2c->periph->CR1 |= I2C_CR1_START;
    while (!(i2c->periph->SR1 & I2C_SR1_SB));
    i2c->periph->DR = address << 1 | I2C_READ_BIT;
    if (length <= 2) {
        i2c->periph->CR1 &= ~I2C_CR1_ACK;
        if (length == 2) {
            i2c->periph->CR1 |= I2C_CR1_POS;
        }
    }
    // EV6 in the datasheet
    while (!(i2c->periph->SR1 & I2C_SR1_ADDR));
    (void)i2c->periph->SR1;
    (void)i2c->periph->SR2;
    if (length == 1) {
        i2c->periph->CR1 |= I2C_CR1_STOP;
        while (!(i2c->periph->SR1 & I2C_SR1_RXNE));
        data[0] = i2c->periph->DR;
    }
    else if (length == 2) {
        while (!(i2c->periph->SR1 & I2C_SR1_BTF));
        i2c->periph->CR1 |= I2C_CR1_STOP;
        data[0] = i2c->periph->DR;
        data[1] = i2c->periph->DR;
        i2c->periph->CR1 &= ~I2C_CR1_POS;
    }
    else {
        for (uint32_t i = 0; i < length; i++) {
            if (i == length - 3) {
                while (!(i2c->periph->SR1 & I2C_SR1_BTF));
                i2c->periph->CR1 &= ~I2C_CR1_ACK;
            }
            else if (i == length - 2) {
                while (!(i2c->periph->SR1 & I2C_SR1_BTF));
                i2c->periph->CR1 |= I2C_CR1_STOP;
            }
            while (!(i2c->periph->SR1 & I2C_SR1_RXNE));
            data[i] = i2c->periph->DR;
        }
    }
    while (i2c->periph->SR2 & I2C_SR2_MSL);
}