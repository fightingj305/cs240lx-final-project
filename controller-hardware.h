#include "config.h"
#ifndef DRONE_CODE

#pragma once
#include "usart.h"
#include "pin.h"
#include "spi.h"
#include "i2c.h"
#include "adc.h"
#include "lsm6ds3.h"
#include "nrf24l01p.h"

extern Pin test_pin;

extern Pin usart2_tx;
extern Pin usart2_rx;
extern USART usart2;

extern Pin spi2_cs;
extern Pin spi2_sck;
extern Pin spi2_miso;
extern Pin spi2_mosi;
extern SPI spi2;

extern Pin adc_x_pin;
extern ADC_Pin adc_x; // left right
extern Pin adc_y_pin;
extern ADC_Pin adc_y; // forward backward
extern Pin adc_z_pin; 
extern ADC_Pin adc_z; // up down
extern Pin adc_yaw_pin; 
extern ADC_Pin adc_yaw; // yaw

extern Pin start_pin;

extern Pin red_pin;
extern Pin green_pin;
extern Pin blue_pin;

extern Pin nrf_ce_pin;
extern NRF24L01 nrf24l01;

#endif