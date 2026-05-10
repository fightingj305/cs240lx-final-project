#pragma once

#include <stdint.h>
#include "pin.h"

#define USART1 ((USART_Regs *)0x40011000U)
#define USART2 ((USART_Regs *)0x40004400U)
#define USART6 ((USART_Regs *)0x40011400U)

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_Regs;

typedef enum {
    USART_BAUD_RATE_9600,
    USART_BAUD_RATE_19200,
    USART_BAUD_RATE_38400,
    USART_BAUD_RATE_57600,
    USART_BAUD_RATE_115200
} USART_Baud_Rate;

typedef struct {
    USART_Regs *periph;
    Pin *tx;
    Pin *rx;
    USART_Baud_Rate baud_rate;
} USART;

void USART_Init(USART *usart);
void USART_Send_Byte(USART *usart, uint8_t byte);
uint8_t USART_Receive_Byte(USART *usart);
void USART_Write(USART *usart, const uint8_t *data, uint32_t length);
void USART_Read(USART *usart, uint8_t *data, uint32_t length);

// Status Register (USART_SR)
enum USART_SR_Bits {
    USART_SR_PE   = (1 << 0),   // Parity error
    USART_SR_FE   = (1 << 1),   // Framing error
    USART_SR_NF   = (1 << 2),   // Noise detected flag
    USART_SR_ORE  = (1 << 3),   // Overrun error
    USART_SR_IDLE = (1 << 4),   // IDLE line detected
    USART_SR_RXNE = (1 << 5),   // Read data register not empty
    USART_SR_TC   = (1 << 6),   // Transmission complete
    USART_SR_TXE  = (1 << 7),   // Transmit data register empty
    USART_SR_LBD  = (1 << 8),   // LIN break detection flag
    USART_SR_CTS  = (1 << 9)    // CTS flag
};

// Data Register (USART_DR)
enum USART_DR_Bits {
    USART_DR_MASK = 0x1FF  // Data value (bits 8:0, bit 8 only if 9-bit mode)
};

// Baud Rate Register (USART_BRR)
enum USART_BRR_Bits {
    USART_BRR_DIV_FRACTION_MASK = (0xF << 0),   // Fraction of USARTDIV
    USART_BRR_DIV_MANTISSA_BIT  = 4,
    USART_BRR_DIV_MANTISSA_MASK = (0xFFF << USART_BRR_DIV_MANTISSA_BIT)
};

// Control Register 1 (USART_CR1)
enum USART_CR1_Bits {
    USART_CR1_SBK    = (1 << 0),   // Send break
    USART_CR1_RWU    = (1 << 1),   // Receiver wakeup
    USART_CR1_RE     = (1 << 2),   // Receiver enable
    USART_CR1_TE     = (1 << 3),   // Transmitter enable
    USART_CR1_IDLEIE = (1 << 4),   // IDLE interrupt enable
    USART_CR1_RXNEIE = (1 << 5),   // RXNE interrupt enable
    USART_CR1_TCIE   = (1 << 6),   // Transmission complete interrupt enable
    USART_CR1_TXEIE  = (1 << 7),   // TXE interrupt enable
    USART_CR1_PEIE   = (1 << 8),   // PE interrupt enable
    USART_CR1_PS     = (1 << 9),   // Parity selection (0=even, 1=odd)
    USART_CR1_PCE    = (1 << 10),  // Parity control enable
    USART_CR1_WAKE   = (1 << 11),  // Wakeup method (0=idle, 1=address mark)
    USART_CR1_M      = (1 << 12),  // Word length (0=8 bits, 1=9 bits)
    USART_CR1_UE     = (1 << 13),  // USART enable
    USART_CR1_OVER8  = (1 << 15)   // Oversampling mode (0=16, 1=8)
};

// Control Register 2 (USART_CR2)
enum USART_CR2_Bits {
    USART_CR2_ADD_MASK   = (0xF << 0),  // Address of USART node
    USART_CR2_LBDL       = (1 << 5),    // LIN break detection length
    USART_CR2_LBDIE      = (1 << 6),    // LIN break detection interrupt enable
    USART_CR2_LBCL       = (1 << 8),    // Last bit clock pulse
    USART_CR2_CPHA       = (1 << 9),    // Clock phase
    USART_CR2_CPOL       = (1 << 10),   // Clock polarity
    USART_CR2_CLKEN      = (1 << 11),   // Clock enable
    
    // Stop bits
    USART_CR2_STOP_1     = (0 << 12),   // 1 stop bit
    USART_CR2_STOP_0_5   = (1 << 12),   // 0.5 stop bit
    USART_CR2_STOP_2     = (2 << 12),   // 2 stop bits
    USART_CR2_STOP_1_5   = (3 << 12),   // 1.5 stop bits
    USART_CR2_STOP_MASK  = (3 << 12),
    
    USART_CR2_LINEN      = (1 << 14)    // LIN mode enable
};

// Control Register 3 (USART_CR3)
enum USART_CR3_Bits {
    USART_CR3_EIE    = (1 << 0),   // Error interrupt enable
    USART_CR3_IREN   = (1 << 1),   // IrDA mode enable
    USART_CR3_IRLP   = (1 << 2),   // IrDA low-power
    USART_CR3_HDSEL  = (1 << 3),   // Half-duplex selection
    USART_CR3_NACK   = (1 << 4),   // Smartcard NACK enable
    USART_CR3_SCEN   = (1 << 5),   // Smartcard mode enable
    USART_CR3_DMAR   = (1 << 6),   // DMA enable receiver
    USART_CR3_DMAT   = (1 << 7),   // DMA enable transmitter
    USART_CR3_RTSE   = (1 << 8),   // RTS enable
    USART_CR3_CTSE   = (1 << 9),   // CTS enable
    USART_CR3_CTSIE  = (1 << 10),  // CTS interrupt enable
    USART_CR3_ONEBIT = (1 << 11)   // One sample bit method enable
};

// Guard Time and Prescaler Register (USART_GTPR)
enum USART_GTPR_Bits {
    USART_GTPR_PSC_MASK = (0xFF << 0),  // Prescaler value
    USART_GTPR_GT_MASK  = (0xFF << 8)   // Guard time value
};
