#include "hardware.h"

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
