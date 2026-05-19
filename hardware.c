#include "hardware.h"

Pin test_pin = {
    .port = GPIOA,
    .pin_num = 4
};

Pin ccw_pin_1 = {
    .port = GPIOA,
    .pin_num = 8
};

Pin cw_pin_1 = {
    .port = GPIOA,
    .pin_num = 9
};

Pin ccw_pin_2 = {
    .port = GPIOA,
    .pin_num = 10
};

Pin cw_pin_2 = {
    .port = GPIOA,
    .pin_num = 11
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

Pin i2c1_scl = {
    .port = GPIOB,
    .pin_num = 6
};

Pin i2c1_sda = {
    .port = GPIOB,
    .pin_num = 7
};

I2C i2c1 = {
    .periph = I2C1,
    .speed = I2C_SPEED_FAST,
    .scl = &i2c1_scl,
    .sda = &i2c1_sda
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
    .divider = SPI_DIV_256,
    .mode = SPI_MODE_MASTER,
    .cpol = SPI_CPOL_HIGH,
    .cpha = SPI_CPHA_1,
    .little_endian = false
};

Pin spi5_sck = {
    .port = GPIOB,
    .pin_num = 0
};

Pin spi5_miso= {
    .port = GPIOA,
    .pin_num = 12
};

Pin spi5_mosi= {
    .port = GPIOB,
    .pin_num = 8
};

Pin spi5_cs = {
    .port = GPIOB,
    .pin_num = 1
};

SPI spi5 = {
    .periph = SPI5,
    .sck = &spi5_sck,
    .miso = &spi5_miso,
    .mosi = &spi5_mosi,
    .cs = &spi5_cs,
    .divider = SPI_DIV_16,
    .mode = SPI_MODE_MASTER,
    .cpol = SPI_CPOL_HIGH,
    .cpha = SPI_CPHA_1,
    .little_endian = false
};

LSM6DS3 lsm6ds3 = {
    .spi = &spi2
};