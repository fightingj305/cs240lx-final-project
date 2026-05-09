#include "spi.h"
#include "rcc.h"

void SPI_Init(SPI *spi) {
    switch ((uint32_t)spi->periph) {
        case (uint32_t)SPI1:
            Pin_Config_AF(spi->sck, PIN_AF5, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(spi->miso, PIN_AF5, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(spi->mosi, PIN_AF5, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            RCC->APB2ENR |= RCC_APB2ENR_SPI1_EN;
            break;
        case (uint32_t)SPI2:
            Pin_Config_AF(spi->sck, PIN_AF5, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(spi->miso, PIN_AF5, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(spi->mosi, PIN_AF5, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            RCC->APB1ENR |= RCC_APB1ENR_SPI2_EN;
            break;
        case (uint32_t)SPI3:
            Pin_Config_AF(spi->sck, PIN_AF6, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(spi->miso, PIN_AF6, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(spi->mosi, PIN_AF6, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            RCC->APB1ENR |= RCC_APB1ENR_SPI3_EN;
            break;
        case (uint32_t)SPI4:
            Pin_AF af = PIN_AF6;
            if (spi->miso->port == GPIOE) {
                af = PIN_AF5;
            }
            Pin_Config_AF(spi->sck, af, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(spi->miso, af, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(spi->mosi, af, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            RCC->APB2ENR |= RCC_APB2ENR_SPI4_EN;
            break;
        case (uint32_t)SPI5:
            Pin_Config_AF(spi->sck, PIN_AF6, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(spi->miso, PIN_AF6, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            Pin_Config_AF(spi->mosi, PIN_AF6, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
            RCC->APB2ENR |= RCC_APB2ENR_SPI5_EN;
            break;
        default:
            PANIC("Invalid SPI peripheral");
    }
    spi->periph->CR1 = (spi->little_endian << SPI_CR1_LSB_BIT ) | 
                        (spi->divider << SPI_CR1_BR_BIT) | 
                        (spi->mode << SPI_CR1_MSTR_BIT) | 
                        (spi->cpol << SPI_CR1_CPOL_BIT) | 
                        (spi->cpha << SPI_CR1_CPHA_BIT) |
                        SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE;

    Pin_Config(spi->cs, PIN_MODE_OUTPUT, PIN_OT_PUSH_PULL, PIN_SPEED_HIGH, PIN_PULL_NONE);
    SPI_Reset_CS(spi); // deselect the slave by default
}

void SPI_Transfer(SPI *spi, uint8_t *tx, uint8_t *rx, uint32_t length) {
    ASSERT(tx != NULL);
    ASSERT(rx != NULL);
    SPI_Set_CS(spi);
    for (uint32_t i = 0; i < length; i++) {
        while (!(spi->periph->SR & SPI_SR_TXE));
        spi->periph->DR = tx[i];
        while (!(spi->periph->SR & SPI_SR_RXNE));
        rx[i] = spi->periph->DR;
    }
    SPI_Reset_CS(spi);
}

void SPI_Read(SPI *spi, uint8_t *data, uint32_t length) {
    ASSERT(data != NULL);
    SPI_Set_CS(spi);
    for (uint32_t i = 0; i < length; i++) {
        while (!(spi->periph->SR & SPI_SR_TXE));
        spi->periph->DR = 0xFF;
        while (!(spi->periph->SR & SPI_SR_RXNE));
        data[i] = spi->periph->DR;
    }
    SPI_Reset_CS(spi);
}

void SPI_Write(SPI *spi, const uint8_t *data, uint32_t length) {
    ASSERT(data != NULL);
    SPI_Set_CS(spi);
    for (uint32_t i = 0; i < length; i++) {
        while (!(spi->periph->SR & SPI_SR_TXE));
        spi->periph->DR = data[i];
        while (!(spi->periph->SR & SPI_SR_RXNE));
        volatile uint8_t dummy = spi->periph->DR;
    }
    SPI_Reset_CS(spi);
}