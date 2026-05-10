#pragma once
#include "usart.h"
#include "pin.h"
#include "spi.h"

extern Pin test_pin;

extern Pin usart2_tx;
extern Pin usart2_rx;
extern USART usart2;

extern Pin spi2_cs;
extern Pin spi2_sck;
extern Pin spi2_miso;
extern Pin spi2_mosi;
extern SPI spi2;