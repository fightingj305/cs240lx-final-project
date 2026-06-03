#pragma once
#include <stdint.h>
#include "pin.h"

#define I2C1 ((I2C_Regs *)0x40005400U)
#define I2C2 ((I2C_Regs *)0x40005800U)
#define I2C3 ((I2C_Regs *)0x40005C00U)

#define I2C_WRITE_BIT 0
#define I2C_READ_BIT  1

typedef struct {
    volatile uint32_t CR1;      // Control register 1
    volatile uint32_t CR2;      // Control register 2
    volatile uint32_t OAR1;     // Own address register 1
    volatile uint32_t OAR2;     // Own address register 2
    volatile uint32_t DR;       // Data register
    volatile uint32_t SR1;      // Status register 1
    volatile uint32_t SR2;      // Status register 2
    volatile uint32_t CCR;      // Clock control register
    volatile uint32_t TRISE;    // TRISE register
    volatile uint32_t FLTR;     // Filter register
} I2C_Regs;

typedef enum {
    I2C_SPEED_STANDARD = 0x00, // 100 kHz
    I2C_SPEED_FAST = 0x01      // 400 kHz
} I2C_Speed;

typedef struct {
    I2C_Regs *periph;
    I2C_Speed speed;
    Pin *scl;
    Pin *sda;
} I2C;

void I2C_Init(I2C *i2c);
void I2C_Write(I2C *i2c, uint8_t address, const uint8_t *data, uint32_t length);
void I2C_Read(I2C *i2c, uint8_t address, uint8_t *data, uint32_t length);
void I2C_WriteRead(I2C *i2c, uint8_t address, 
                   const uint8_t *tx, uint32_t tx_len,
                   uint8_t *rx, uint32_t rx_len);

// Control Register 1 (I2C_CR1)
enum I2C_CR1_Bits {
    I2C_CR1_PE          = (1 << 0),   // Peripheral enable
    I2C_CR1_SMBUS       = (1 << 1),   // SMBus mode
    I2C_CR1_SMBTYPE     = (1 << 3),   // SMBus type
    I2C_CR1_ENARP       = (1 << 4),   // ARP enable
    I2C_CR1_ENPEC       = (1 << 5),   // PEC enable
    I2C_CR1_ENGC        = (1 << 6),   // General call enable
    I2C_CR1_NOSTRETCH   = (1 << 7),   // Clock stretching disable (Slave mode)
    I2C_CR1_START       = (1 << 8),   // Start generation
    I2C_CR1_STOP        = (1 << 9),   // Stop generation
    I2C_CR1_ACK         = (1 << 10),  // Acknowledge enable
    I2C_CR1_POS         = (1 << 11),  // Acknowledge/PEC position
    I2C_CR1_PEC         = (1 << 12),  // Packet error checking
    I2C_CR1_ALERT       = (1 << 13),  // SMBus alert
    I2C_CR1_SWRST       = (1 << 15)   // Software reset
};

// Control Register 2 (I2C_CR2)
enum I2C_CR2_Bits {
    I2C_CR2_FREQ_MASK   = (0x3F << 0), // Peripheral clock frequency (2-50 MHz)
    I2C_CR2_ITERREN     = (1 << 8),    // Error interrupt enable
    I2C_CR2_ITEVTEN     = (1 << 9),    // Event interrupt enable
    I2C_CR2_ITBUFEN     = (1 << 10),   // Buffer interrupt enable
    I2C_CR2_DMAEN       = (1 << 11),   // DMA requests enable
    I2C_CR2_LAST        = (1 << 12)    // DMA last transfer
};

// Own Address Register 1 (I2C_OAR1)
enum I2C_OAR1_Bits {
    I2C_OAR1_ADD0        = (1 << 0),    // Interface address bit 0 (10-bit mode)
    I2C_OAR1_ADD7_1_MASK = (0x7F << 1), // Interface address bits 7:1
    I2C_OAR1_ADD9_8_MASK = (0x3 << 8),  // Interface address bits 9:8 (10-bit mode)
    I2C_OAR1_RESERVED    = (0x1 << 14),
    I2C_OAR1_ADDMODE     = (1 << 15)    // Addressing mode (0=7-bit, 1=10-bit)
};

// Own Address Register 2 (I2C_OAR2)
enum I2C_OAR2_Bits {
    I2C_OAR2_ENDUAL     = (1 << 0),    // Dual addressing mode enable
    I2C_OAR2_ADD2_MASK  = (0x7F << 1)  // Interface address bits 7:1
};

// Status Register 1 (I2C_SR1)
enum I2C_SR1_Bits {
    I2C_SR1_SB          = (1 << 0),    // Start bit (Master mode)
    I2C_SR1_ADDR        = (1 << 1),    // Address sent/matched
    I2C_SR1_BTF         = (1 << 2),    // Byte transfer finished
    I2C_SR1_ADD10       = (1 << 3),    // 10-bit header sent (Master mode)
    I2C_SR1_STOPF       = (1 << 4),    // Stop detection (Slave mode)
    I2C_SR1_RXNE        = (1 << 6),    // Data register not empty (receivers)
    I2C_SR1_TXE         = (1 << 7),    // Data register empty (transmitters)
    I2C_SR1_BERR        = (1 << 8),    // Bus error
    I2C_SR1_ARLO        = (1 << 9),    // Arbitration lost
    I2C_SR1_AF          = (1 << 10),   // Acknowledge failure
    I2C_SR1_OVR         = (1 << 11),   // Overrun/Underrun
    I2C_SR1_PECERR      = (1 << 12),   // PEC Error in reception
    I2C_SR1_TIMEOUT     = (1 << 14),   // Timeout or Tlow error
    I2C_SR1_SMBALERT    = (1 << 15)    // SMBus alert
};

// Status Register 2 (I2C_SR2)
enum I2C_SR2_Bits {
    I2C_SR2_MSL         = (1 << 0),    // Master/slave
    I2C_SR2_BUSY        = (1 << 1),    // Bus busy
    I2C_SR2_TRA         = (1 << 2),    // Transmitter/receiver
    I2C_SR2_GENCALL     = (1 << 4),    // General call address (Slave mode)
    I2C_SR2_SMBDEFAULT  = (1 << 5),    // SMBus device default address (Slave mode)
    I2C_SR2_SMBHOST     = (1 << 6),    // SMBus host header (Slave mode)
    I2C_SR2_DUALF       = (1 << 7),    // Dual flag (Slave mode)
    I2C_SR2_PEC_MASK    = (0xFF << 8)  // Packet error checking register
};

// Clock Control Register (I2C_CCR)
enum I2C_CCR_Bits {
    I2C_CCR_CCR_MASK    = (0xFFF << 0), // Clock control register in Fast/Standard mode
    I2C_CCR_DUTY        = (1 << 14),    // Fast mode duty cycle (0=2:1, 1=16:9)
    I2C_CCR_FS          = (1 << 15)     // I2C master mode selection (0=Sm, 1=Fm)
};

// TRISE Register (I2C_TRISE)
enum I2C_TRISE_Bits {
    I2C_TRISE_TRISE_MASK = (0x3F << 0)  // Maximum rise time in Fast/Standard mode
};

// Filter Register (I2C_FLTR)
enum I2C_FLTR_Bits {
    I2C_FLTR_DNF_MASK   = (0xF << 0),   // Digital noise filter
    I2C_FLTR_ANOFF      = (1 << 4)      // Analog noise filter OFF
};
