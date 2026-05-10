#include "hardware.h"

Pin test_pin = {
    .port = GPIOA,
    .pin_num = 8
};

Pin usart2_tx = {
    .port = GPIOA,
    .pin_num = 2
};

Pin usart2_rx = {
    .port = GPIOA,
    .pin_num = 3
};

USART usart2 = {
    .periph = USART2,
    .tx = &usart2_tx,
    .rx = &usart2_rx,
    .baud_rate = USART_BAUD_RATE_115200
};

Pin spi2_cs = {
    .port = GPIOB,
    .pin_num = 12
};

Pin spi2_sck = {
    .port = GPIOB,
    .pin_num = 13
};

Pin spi2_miso= {
    .port = GPIOB,
    .pin_num = 14
};

Pin spi2_mosi= {
    .port = GPIOB,
    .pin_num = 15
};

SPI spi2 = {
    .periph = SPI2,
    .sck = &spi2_sck,
    .miso = &spi2_miso,
    .mosi = &spi2_mosi,
    .cs = &spi2_cs,
    .divider = SPI_DIV_16,
    .mode = SPI_MODE_MASTER,
    .cpol = SPI_CPOL_LOW,
    .cpha = SPI_CPHA_0,
    .little_endian = true
};