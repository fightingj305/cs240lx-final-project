#include "hardware.h"
#include "timer.h"
#include "systick.h"
#include <stdio.h>
#ifdef DRONE_CODE

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
    .cpol = SPI_CPOL_LOW,
    .cpha = SPI_CPHA_0,
    .little_endian = false
};

Pin led_pin = {
    .port = GPIOC,
    .pin_num = 13
};

LSM6DS3 lsm6ds3 = {
    .spi = &spi2
};

Pin nrf_ce_pin = {
    .port = GPIOA,
    .pin_num = 7
};

Pin nrf_irq_pin = {
    .port = GPIOB,
    .pin_num = 2
};

NRF24L01 nrf24l01 = {
    .device = &spi5,
    .ce_pin = &nrf_ce_pin,
    .irq_pin = &nrf_irq_pin,
    .channel = 76,
    .ack = true,
    .nbytes = 10,
    .address_width = NRF_ADDRESS_3_BYTE,
    .power = NRF_PWR_18DBM,
    .datarate = NRF_DR_2MBPS,
    .tx_addr = {0xE7, 0xE7, 0xE7, 0, 0},
};

I2C *vl53_i2c = &i2c1;

void Drone_Hardware_Init() {    
    SysTick_Init();
    SysTick_Delay_Milliseconds(1000);

#ifdef UART_REDIRECT 
    USART_Init(&usart2);
#endif
    TIM1_Init(1000000);
    TIM1_Config_PWM(TIM_CHANNEL_1, &ccw_pin_1);
    TIM1_Config_PWM(TIM_CHANNEL_2, &cw_pin_1);
    TIM1_Config_PWM(TIM_CHANNEL_3, &ccw_pin_2);
    TIM1_Config_PWM(TIM_CHANNEL_4, &cw_pin_2);
    
    Pin_Config(&led_pin, PIN_MODE_OUTPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_NONE);
    Pin_Set_High(&led_pin);
    
    LSM6DS3_Init(&lsm6ds3);
    uint8_t who = LSM6DS3_Read_Reg(&lsm6ds3, 0x0F);
    printf("WHOAMI: %02X\n", who);
    if (who != 0x69) {
        printf("LSM6DS3 init failed, halting\n");
        while (1);
    }
    
    
    NRF_Config(&nrf24l01);
    NRF_Config_RX_IRQ(&nrf24l01);
    uint8_t pipe1_addr[5] = {0xE7, 0xE7, 0xE7, 0x00, 0x00};
    NRF_Setup_Pipe(&nrf24l01, NRF_PIPE1, pipe1_addr);
    uint8_t rf_ch = NRF_Read_Reg(&nrf24l01, NRF_RF_CH);
    printf("RF_CH: %02X\n", rf_ch);  // should be 0x4C
    if (rf_ch != nrf24l01.channel) {
        printf("NRF init failed, halting\n");
        while (1);
    }
    
    I2C_Init(vl53_i2c);

    uint8_t vl53_booted = 0;
    printf("Waiting for VL53L1X...\n");
    while (!vl53_booted) {
        VL53L1X_BootState(VL53L1X_ADDR, &vl53_booted);
        SysTick_Delay_Milliseconds(2);
    }

    uint16_t sensor_id = 0;
    VL53L1X_GetSensorId(VL53L1X_ADDR, &sensor_id);
    printf("VL53L1X sensor ID: %04X\n", sensor_id);
    if (sensor_id != 0xEACC) {
        printf("VL53L1X init failed\n");
        while (1);
    }
    
    VL53L1X_SensorInit(VL53L1X_ADDR);
    VL53L1X_SetDistanceMode(VL53L1X_ADDR, 1);
    VL53L1X_SetTimingBudgetInMs(VL53L1X_ADDR, 20);
    VL53L1X_SetInterMeasurementInMs(VL53L1X_ADDR, 20);
    VL53L1X_StartRanging(VL53L1X_ADDR);
    printf("VL53L1X ready\n");
}

#endif