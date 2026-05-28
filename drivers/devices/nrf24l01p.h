#pragma once 

#include "spi.h"

#define NRF_LARGE_REG_SIZE 5
#define NRF_MAX_PACKET_SIZE 32

enum NRF_Commands {
    NRF_R_REGISTER           = 0x00,  // or with last 5 bits as addr
    NRF_W_REGISTER           = 0x20,  // or with last 5 bits as addr
    NRF_R_RX_PAYLOAD         = 0x61,
    NRF_W_TX_PAYLOAD         = 0xA0,
    NRF_FLUSH_TX             = 0xE1,
    NRF_FLUSH_RX             = 0xE2,
    NRF_REUSE_TX_PL          = 0xE3,
    NRF_R_RX_PL_WID          = 0x60,
    NRF_W_ACK_PAYLOAD        = 0xA8,  // or with last 3 bits as pipe
    NRF_W_TX_PAYLOAD_NOACK   = 0xB0,
    NRF_NOP                  = 0xFF
};

enum NRF_Registers {
    NRF_CONFIG        = 0x00,
    NRF_EN_AA         = 0x01,
    NRF_EN_RXADDR     = 0x02,
    NRF_SETUP_AW      = 0x03,
    NRF_SETUP_RETR    = 0x04,
    NRF_RF_CH         = 0x05,
    NRF_RF_SETUP      = 0x06,
    NRF_STATUS        = 0x07,
    NRF_OBSERVE_TX    = 0x08,
    NRF_RPD           = 0x09,
    NRF_RX_ADDR_P0    = 0x0A,
    NRF_RX_ADDR_P1    = 0x0B,
    NRF_RX_ADDR_P2    = 0x0C,
    NRF_RX_ADDR_P3    = 0x0D,
    NRF_RX_ADDR_P4    = 0x0E,
    NRF_RX_ADDR_P5    = 0x0F,
    NRF_TX_ADDR       = 0x10,
    NRF_RX_PW_P0      = 0x11,
    NRF_RX_PW_P1      = 0x12,
    NRF_RX_PW_P2      = 0x13,
    NRF_RX_PW_P3      = 0x14,
    NRF_RX_PW_P4      = 0x15,
    NRF_RX_PW_P5      = 0x16,
    NRF_FIFO_STATUS   = 0x17,
    NRF_DYNPD         = 0x1C,
    NRF_FEATURE       = 0x1D
};

enum NRF_Config_Bits {
    NRF_CONFIG_PRIM_RX_BIT  = 0,
    NRF_CONFIG_PWR_UP_BIT   = 1,
    NRF_CONFIG_CRCO_BIT     = 2,
    NRF_CONFIG_EN_CRC_BIT   = 3,
    NRF_CONFIG_MASK_MAX_RT_BIT = 4,
    NRF_CONFIG_MASK_TX_DS_BIT  = 5,
    NRF_CONFIG_MASK_RX_DR_BIT  = 6,
};

enum NRF_Status_Bits {
    NRF_STATUS_RX_DR_BIT   = 6,
    NRF_STATUS_TX_DS_BIT   = 5,
    NRF_STATUS_MAX_RT_BIT  = 4,
    NRF_STATUS_RX_P_NO_BIT = 1,
    NRF_STATUS_TX_FULL_BIT = 0,
    NRF_STATUS_RESERVED_BIT = 7
};

typedef enum NRF_Pipe_t {
    NRF_PIPE0 = 0,
    NRF_PIPE1,
    NRF_PIPE2,
    NRF_PIPE3,
    NRF_PIPE4,
    NRF_PIPE5,
    NRF_NUM_PIPES
} NRF_Pipe;

typedef enum NRF_Address_Width_t {
    NRF_ADDRESS_3_BYTE = 0x01,
    NRF_ADDRESS_4_BYTE = 0x02,
    NRF_ADDRESS_5_BYTE = 0x03,
} NRF_Address_Width;

typedef enum NRF_Power_t {
    NRF_PWR_18DBM = 0x00,
    NRF_PWR_12DBM = 0x01,
    NRF_PWR_6DBM = 0x02,
    NRF_PWR_0DBM = 0x03,
    NRF_PWR_BIT = 1
} NRF_Power;

typedef enum NRF_Datarate_t {
    NRF_DR_1MBPS = 0x00,
    NRF_DR_2MBPS = 0x01,
    NRF_DR_250KBPS = 0x02,
    NRF_DR_BIT = 3
} NRF_Datarate;

typedef struct NRF24L01_t { 
    SPI *device;
    Pin *ce_pin;
    Pin *irq_pin;

    uint8_t channel;
    bool ack;
    uint32_t nbytes;

    NRF_Address_Width address_width;
    NRF_Power power;
    NRF_Datarate datarate;
    
    uint8_t tx_addr[NRF_LARGE_REG_SIZE];
    uint8_t rx_data[NRF_NUM_PIPES][NRF_MAX_PACKET_SIZE];
} NRF24L01;

void NRF_Config(NRF24L01 *nrf);
void NRF_Config_RX_IRQ(NRF24L01 *nrf);
void NRF_Setup_Pipe(NRF24L01 *nrf, NRF_Pipe pipe, uint8_t *address);

bool NRF_Send(NRF24L01 *nrf, uint8_t *data);
void NRF_Receive(NRF24L01 *nrf);
void NRF_Receive_Nonblocking(NRF24L01 *nrf);
void NRF_Read_Pipe(NRF24L01 *nrf, NRF_Pipe pipe, uint8_t *data);
void NRF_Set_TX_Mode(NRF24L01 *nrf);
void NRF_Set_RX_Mode(NRF24L01 *nrf);

uint8_t NRF_Get_Status(NRF24L01 *nrf);
bool NRF_Check_RT(NRF24L01 *nrf);
void NRF_Clear_IRQ(NRF24L01 *nrf);

void NRF_Flush_TX(NRF24L01 *nrf);
void NRF_Flush_RX(NRF24L01 *nrf);

void NRF_FIFO_Write_Packet(NRF24L01 *nrf, uint8_t *data);

void NRF_Write_Reg(NRF24L01 *nrf, uint8_t addr, uint8_t value);
void NRF_Write_Large_Reg(NRF24L01 *nrf, uint8_t addr, uint8_t* data);

uint8_t NRF_Read_Reg(NRF24L01 *nrf, uint8_t addr);
void NRF_Read_Large_Reg(NRF24L01 *nrf, uint8_t addr, uint8_t* data);