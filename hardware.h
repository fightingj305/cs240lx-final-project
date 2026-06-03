#include "config.h"
#ifdef DRONE_CODE

#pragma once
#include "usart.h"
#include "pin.h"
#include "spi.h"
#include "i2c.h"
#include "adc.h"
#include "lsm6ds3.h"
#include "nrf24l01p.h"
#include "VL53L1X_api.h"

#define VL53L1X_ADDR 0x52

extern Pin test_pin;

extern Pin ccw_pin_1;
extern Pin cw_pin_1;
extern Pin ccw_pin_2;
extern Pin cw_pin_2;

extern Pin usart2_tx;
extern Pin usart2_rx;
extern USART usart2;

extern Pin i2c1_scl;
extern Pin i2c1_sda;
extern I2C i2c1;

extern Pin spi2_cs;
extern Pin spi2_sck;
extern Pin spi2_miso;
extern Pin spi2_mosi;
extern SPI spi2;

extern Pin spi5_sck;
extern Pin spi5_miso;
extern Pin spi5_mosi;
extern Pin spi5_cs;
extern SPI spi5;

extern Pin led_pin;

extern LSM6DS3 lsm6ds3;

extern Pin nrf_ce_pin;

extern NRF24L01 nrf24l01;

void Drone_Hardware_Init();

#endif