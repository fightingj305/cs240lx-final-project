#pragma once

#include <stdint.h>
#include "pin.h"

#define SPI1 ((SPI_Regs *)0x40013000U) // might conflict with jtag
#define SPI2 ((SPI_Regs *)0x40003800U)
#define SPI3 ((SPI_Regs *)0x40003C00U) // might conflict with jtag
#define SPI4 ((SPI_Regs *)0x40013400U)
#define SPI5 ((SPI_Regs *)0x40015000U)

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
} SPI_Regs;

typedef enum {
    SPI_MODE_SLAVE = 0x00,
    SPI_MODE_MASTER = 0x01
} SPI_Mode;

typedef enum {
    SPI_CPOL_LOW = 0x00, // idle state
    SPI_CPOL_HIGH = 0x01
} SPI_CPOL;

typedef enum {
    SPI_CPHA_0 = 0x00, // data sampled on leading edge
    SPI_CPHA_1 = 0x01 // data sampled on trailing edge
} SPI_CPHA;

typedef enum { // apb2 clock is 84mhz, apb1 clock is 42mhz
    SPI_DIV_2 = 0x00,
    SPI_DIV_4 = 0x01,
    SPI_DIV_8 = 0x02,
    SPI_DIV_16 = 0x03,
    SPI_DIV_32 = 0x04,
    SPI_DIV_64 = 0x05,
    SPI_DIV_128 = 0x06,
    SPI_DIV_256 = 0x07
} SPI_Divider;

typedef struct {
    SPI_Regs *periph;
    Pin *sck;
    Pin *miso;
    Pin *mosi;
    Pin *cs;

    SPI_Divider divider;
    SPI_Mode mode;
    SPI_CPOL cpol;
    SPI_CPHA cpha;
    bool little_endian;
} SPI;

void SPI_Init(SPI *spi);

static inline void SPI_Reset_CS(SPI *spi) {
    Pin_Set(spi->cs, true);
}
static inline void SPI_Set_CS(SPI *spi) {
    Pin_Set(spi->cs, false);
}

void SPI_Transfer(SPI *spi, uint8_t *tx, uint8_t *rx, uint32_t length);
void SPI_Read(SPI *spi, uint8_t *data, uint32_t length);
void SPI_Write(SPI *spi, const uint8_t *data, uint32_t length);

enum SPI_CR1_Bits {
    SPI_CR1_CPHA_BIT    = 0,   // Clock phase
    SPI_CR1_CPOL_BIT    = 1,   // Clock polarity
    SPI_CR1_MSTR_BIT    = 2,   // Master selection

    // Baud rate control
    SPI_CR1_BR_BIT      = 3,
    SPI_CR1_BR_MASK     = (0x7 << SPI_CR1_BR_BIT),
    
    SPI_CR1_SPE         = (1 << 6),   // SPI enable
    SPI_CR1_LSB_BIT     = 7,          // Frame endianess (0=MSB, 1=LSB)
    SPI_CR1_SSI         = (1 << 8),   // Internal slave select
    SPI_CR1_SSM         = (1 << 9),   // Software slave management
    SPI_CR1_RXONLY      = (1 << 10),  // Receive only
    SPI_CR1_DFF         = (1 << 11),  // Data frame format (0=8-bit, 1=16-bit)
    SPI_CR1_CRCNEXT     = (1 << 12),  // CRC transfer next
    SPI_CR1_CRCEN       = (1 << 13),  // Hardware CRC calculation enable
    SPI_CR1_BIDIOE      = (1 << 14),  // Output enable in bidirectional mode
    SPI_CR1_BIDIMODE    = (1 << 15)   // Bidirectional data mode enable
};

// SPI Control Register 2 (SPI_CR2)
enum SPI_CR2_Bits {
    SPI_CR2_RXDMAEN     = (1 << 0),   // Rx buffer DMA enable
    SPI_CR2_TXDMAEN     = (1 << 1),   // Tx buffer DMA enable
    SPI_CR2_SSOE        = (1 << 2),   // SS output enable
    SPI_CR2_FRF         = (1 << 4),   // Frame format (0=Motorola, 1=TI)
    SPI_CR2_ERRIE       = (1 << 5),   // Error interrupt enable
    SPI_CR2_RXNEIE      = (1 << 6),   // RX buffer not empty interrupt enable
    SPI_CR2_TXEIE       = (1 << 7)    // Tx buffer empty interrupt enable
};

// SPI Status Register (SPI_SR)
enum SPI_SR_Bits {
    SPI_SR_RXNE         = (1 << 0),   // Receive buffer not empty
    SPI_SR_TXE          = (1 << 1),   // Transmit buffer empty
    SPI_SR_CHSIDE       = (1 << 2),   // Channel side (I2S mode)
    SPI_SR_UDR          = (1 << 3),   // Underrun flag (I2S mode)
    SPI_SR_CRCERR       = (1 << 4),   // CRC error flag
    SPI_SR_MODF         = (1 << 5),   // Mode fault
    SPI_SR_OVR          = (1 << 6),   // Overrun flag
    SPI_SR_BSY          = (1 << 7),   // Busy flag
    SPI_SR_FRE          = (1 << 8)    // Frame format error (TI mode)
};

