#include "nrf24l01p.h"
#include "utils.h"
#include "systick.h"

void NRF_Config(NRF24L01 *nrf) {
    ASSERT((nrf->channel >> 7) == 0); // require bit 7 to be 0
    ASSERT(nrf->nbytes <= 32);
    SPI_Init(nrf->device);
    Pin_Config(nrf->ce_pin, PIN_MODE_OUTPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_NONE);
    Pin_Set_Low(nrf->ce_pin);
    NRF_Flush_RX(nrf);
    NRF_Flush_TX(nrf);
    NRF_Write_Reg(nrf, NRF_RF_CH, nrf->channel);
    NRF_Write_Reg(nrf, NRF_SETUP_AW, nrf->address_width);
    NRF_Write_Reg(nrf, NRF_RF_SETUP, (nrf->power << NRF_PWR_BIT) | (nrf->datarate << NRF_DR_BIT));
    NRF_Write_Large_Reg(nrf, NRF_TX_ADDR, nrf->tx_addr);
    NRF_Write_Reg(nrf, NRF_CONFIG, (1 << NRF_CONFIG_PWR_UP_BIT) | (1 << NRF_CONFIG_EN_CRC_BIT) | (1 << NRF_CONFIG_CRCO_BIT));
    SysTick_Delay_Microseconds(5000);

    if (nrf->ack) {
        NRF_Setup_Pipe(nrf, NRF_PIPE0, nrf->tx_addr);
        NRF_Write_Reg(nrf, NRF_SETUP_RETR, 0x3F);
    } else {
        NRF_Write_Reg(nrf, NRF_EN_AA, 0x00);
        NRF_Write_Reg(nrf, NRF_SETUP_RETR, 0x00);
    }
}

void NRF_Setup_Pipe(NRF24L01 *nrf, NRF_Pipe pipe, uint8_t *address) {
    NRF_Write_Large_Reg(nrf, (uint8_t)NRF_RX_ADDR_P0 + (uint8_t)pipe, address);
    uint8_t old_pipe_status = NRF_Read_Reg(nrf, NRF_EN_RXADDR);
    NRF_Write_Reg(nrf, NRF_EN_RXADDR, old_pipe_status | (1 << pipe));
    NRF_Write_Reg(nrf, (uint8_t)NRF_RX_PW_P0 + (uint8_t)pipe, nrf->nbytes);
    if (nrf->ack) {
        uint8_t old_aa_status = NRF_Read_Reg(nrf, NRF_EN_AA);
        NRF_Write_Reg(nrf, NRF_EN_AA, old_aa_status | (1 << pipe));
    }
}

void NRF_FIFO_Write_Packet(NRF24L01 *nrf, uint8_t *data) {
    uint8_t bytes_to_write[NRF_MAX_PACKET_SIZE + 1];
    bytes_to_write[0] = nrf->ack ? NRF_W_TX_PAYLOAD : NRF_W_TX_PAYLOAD_NOACK;
    for (uint32_t i = 0; i < nrf->nbytes; i++) {
        bytes_to_write[i+1] = data[i];
    }
    SPI_Write(nrf->device, bytes_to_write, nrf->nbytes + 1);
    SysTick_Delay_Microseconds(5);
}

void NRF_FIFO_Read_Packet(NRF24L01 *nrf, uint8_t *data) {
    uint8_t bytes_to_write[NRF_MAX_PACKET_SIZE + 1];
    bytes_to_write[0] = NRF_R_RX_PAYLOAD;
    for (uint32_t i = 0; i < nrf->nbytes; i++) {
        bytes_to_write[i+1] = 0xFF;
    }
    uint8_t data_received[NRF_MAX_PACKET_SIZE + 1];
    SPI_Transfer(nrf->device, bytes_to_write, data_received, nrf->nbytes + 1);
    for (uint32_t i = 0; i < nrf->nbytes; i++) {
        data[i] = data_received[i+1];
    }
    SysTick_Delay_Microseconds(5);
}

void NRF_Send(NRF24L01 *nrf, uint8_t *data) {
    NRF_FIFO_Write_Packet(nrf, data);

    NRF_Set_TX_Mode(nrf);
    Pin_Set_High(nrf->ce_pin);
    SysTick_Delay_Microseconds(150);

    uint8_t status;
    while (1) {
        status = NRF_Get_Status(nrf);
        if (status & (1 << NRF_STATUS_TX_DS_BIT)) break;
        if (status & (1 << NRF_STATUS_MAX_RT_BIT)) {
            printf("MAX_RT hit!\n");
            NRF_Write_Reg(nrf, NRF_STATUS, 1 << NRF_STATUS_MAX_RT_BIT);
            NRF_Flush_TX(nrf);
            break;
        }
    }
    NRF_Write_Reg(nrf, NRF_STATUS, 1 << NRF_STATUS_TX_DS_BIT);
    Pin_Set_Low(nrf->ce_pin);
}

void NRF_Receive(NRF24L01 *nrf) {
    NRF_Flush_RX(nrf);
    NRF_Set_RX_Mode(nrf);

    while (!(NRF_Get_Status(nrf) & (1 << NRF_STATUS_RX_DR_BIT))) {
        SysTick_Delay_Microseconds(10);
    }

    while (NRF_Get_Status(nrf) & (1 << NRF_STATUS_RX_DR_BIT)) {
        uint8_t status = NRF_Get_Status(nrf);
        NRF_Pipe pipe = (NRF_Pipe) ((status >> NRF_STATUS_RX_P_NO_BIT) & 0x07);
        NRF_FIFO_Read_Packet(nrf, nrf->rx_data[pipe]);
        NRF_Write_Reg(nrf, NRF_STATUS, 1 << NRF_STATUS_RX_DR_BIT);
    }
    Pin_Set_Low(nrf->ce_pin);
    SysTick_Delay_Microseconds(150);
}

void NRF_Set_TX_Mode(NRF24L01 *nrf) {
    uint8_t config = NRF_Read_Reg(nrf, NRF_CONFIG);
    NRF_Write_Reg(nrf, NRF_CONFIG, config & ~(1 << NRF_CONFIG_PRIM_RX_BIT));
    Pin_Set_Low(nrf->ce_pin);
    SysTick_Delay_Microseconds(150);
}

void NRF_Set_RX_Mode(NRF24L01 *nrf) {
    uint8_t config = NRF_Read_Reg(nrf, NRF_CONFIG);
    NRF_Write_Reg(nrf, NRF_CONFIG, config | (1 << NRF_CONFIG_PRIM_RX_BIT));
    Pin_Set_High(nrf->ce_pin);
    SysTick_Delay_Microseconds(150);
}

void NRF_Read_Pipe(NRF24L01 *nrf, NRF_Pipe pipe, uint8_t *data) {
    for (uint32_t i = 0; i < nrf->nbytes; i++) {
        data[i] = nrf->rx_data[pipe][i];
    }
}

uint8_t NRF_Get_Status(NRF24L01 *nrf) {
    return SPI_Transfer_Byte(nrf->device, NRF_NOP);
}


void NRF_Flush_TX(NRF24L01 *nrf) {
    SPI_Write_Byte(nrf->device, NRF_FLUSH_TX);
}
void NRF_Flush_RX(NRF24L01 *nrf) {
    SPI_Write_Byte(nrf->device, NRF_FLUSH_RX);
}

void NRF_Write_Reg(NRF24L01 *nrf, uint8_t addr, uint8_t value) {
    ASSERT(addr == (addr & 0x1F));
    uint8_t tx[2] = { NRF_W_REGISTER | addr, value };
    uint8_t rx[2];
    SPI_Transfer(nrf->device, tx, rx, 2);
    SysTick_Delay_Microseconds(5);
}

void NRF_Write_Large_Reg(NRF24L01 *nrf, uint8_t addr, uint8_t* data) {
    uint32_t reg_size = nrf->address_width + 2;
    ASSERT(addr == (addr & 0x1F)); // addresses should be 5 bit

    uint8_t bytes_to_write[NRF_LARGE_REG_SIZE + 1];
    bytes_to_write[0] = NRF_W_REGISTER | addr;
    for (int i = 0; i < reg_size; i++) {
        bytes_to_write[i+1] = data[i];
    }

    uint8_t data_received[NRF_LARGE_REG_SIZE + 1];
    SPI_Transfer(nrf->device, bytes_to_write, data_received, reg_size + 1);
    SysTick_Delay_Microseconds(5);    
}

void NRF_Read_Large_Reg(NRF24L01 *nrf, uint8_t addr, uint8_t* data) {
    uint32_t reg_size = nrf->address_width + 2;
    ASSERT(addr == (addr & 0x1F)); // addresses should be 5 bit
    
    uint8_t bytes_to_write[NRF_LARGE_REG_SIZE + 1] = {NRF_R_REGISTER | addr, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t data_received[NRF_LARGE_REG_SIZE + 1];
    SPI_Transfer(nrf->device, bytes_to_write, data_received, reg_size + 1);
    SysTick_Delay_Microseconds(5);
    for (int i = 0; i < reg_size; i++) {
        data[i] = data_received[i+1];
    }
}

uint8_t NRF_Read_Reg(NRF24L01 *nrf, uint8_t addr) {
    ASSERT(addr == (addr & 0x1F));
    uint8_t tx[2] = { NRF_R_REGISTER | addr, NRF_NOP };
    uint8_t rx[2];
    SPI_Transfer(nrf->device, tx, rx, 2);
    SysTick_Delay_Microseconds(5);
    return rx[1];
}