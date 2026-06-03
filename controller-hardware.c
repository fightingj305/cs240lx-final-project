#include "controller-hardware.h"
#ifndef DRONE_CODE
#include "systick.h"
#include <stdio.h>
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
    .little_endian = false
};

Pin adc_x_pin = {
    .port = GPIOA,
    .pin_num = 5
};

ADC_Pin adc_x = {
    .pin = &adc_x_pin,
    .channel = ADC1_CH5
};

Pin adc_y_pin = {
    .port = GPIOA,
    .pin_num = 4
};

ADC_Pin adc_y = {
    .pin = &adc_y_pin,
    .channel = ADC1_CH4
};

Pin adc_z_pin = {
    .port = GPIOA,
    .pin_num = 7
};

ADC_Pin adc_z = {
    .pin = &adc_z_pin,
    .channel = ADC1_CH7
};


Pin adc_yaw_pin = {
    .port = GPIOA,
    .pin_num = 6
}; 
ADC_Pin adc_yaw = {
    .pin = &adc_yaw_pin,
    .channel = ADC1_CH6
};

Pin start_pin = {
    .port = GPIOA,
    .pin_num = 15
};

Pin red_pin = {
    .port = GPIOB,
    .pin_num = 6
};

Pin green_pin = {
    .port = GPIOB,
    .pin_num = 4
};

Pin blue_pin = {
    .port = GPIOA,
    .pin_num = 0
};

Pin nrf_ce_pin = {
    .port = GPIOB,
    .pin_num = 8
};

NRF24L01 nrf24l01 = {
    .device = &spi2,
    .ce_pin = &nrf_ce_pin,
    .channel = 76,
    .ack = true,
    .nbytes = 10,
    .address_width = NRF_ADDRESS_3_BYTE,
    .power = NRF_PWR_18DBM,
    .datarate = NRF_DR_2MBPS,
    .tx_addr = {0xE7, 0xE7, 0xE7, 0, 0},
};

void Controller_Hardware_Init() {
    SysTick_Init();
    SysTick_Delay_Milliseconds(1000); // startup delay

#ifdef UART_REDIRECT
    USART_Init(&usart2);
#endif
    
    NRF_Config(&nrf24l01);
    uint8_t pipe1_addr[5] = {0xE7, 0xE7, 0xE7, 0x00, 0x00};
    NRF_Setup_Pipe(&nrf24l01, NRF_PIPE1, pipe1_addr);
    uint8_t rf_ch = NRF_Read_Reg(&nrf24l01, NRF_RF_CH);
    printf("RF_CH: %02X\n", rf_ch);  // should be 0x4C
    if (rf_ch != nrf24l01.channel) {
        printf("NRF init failed, halting\n");
        while (1);
    }
    
    ADC_Init();
    ADC_Config_Pin(&adc_x);
    ADC_Config_Pin(&adc_y);
    ADC_Config_Pin(&adc_z);
    ADC_Config_Pin(&adc_yaw);
    
    Pin_Config(&start_pin, PIN_MODE_INPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_UP);
    Pin_Config(&red_pin, PIN_MODE_OUTPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_NONE);
    Pin_Config(&green_pin, PIN_MODE_OUTPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_NONE);
    Pin_Config(&blue_pin, PIN_MODE_OUTPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_NONE);
}

#endif