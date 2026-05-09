#pragma once
#include <stdint.h>

#define FLASH ((FLASH_Regs *)0x40023C00U)

typedef struct {
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t OPTCR;
} FLASH_Regs;

// Flash access control register (FLASH_ACR)
enum FLASH_ACR_Bits {
    // Latency
    FLASH_ACR_LATENCY_0WS = (0 << 0),
    FLASH_ACR_LATENCY_1WS = (1 << 0),
    FLASH_ACR_LATENCY_2WS = (2 << 0),
    FLASH_ACR_LATENCY_3WS = (3 << 0),
    FLASH_ACR_LATENCY_4WS = (4 << 0),
    FLASH_ACR_LATENCY_5WS = (5 << 0),
    FLASH_ACR_LATENCY_6WS = (6 << 0),
    FLASH_ACR_LATENCY_7WS = (7 << 0),
    FLASH_ACR_LATENCY_MASK = (0xF << 0),
    
    FLASH_ACR_PRFTEN  = (1 << 8),   // Prefetch enable
    FLASH_ACR_ICEN    = (1 << 9),   // Instruction cache enable
    FLASH_ACR_DCEN    = (1 << 10),  // Data cache enable
    FLASH_ACR_ICRST   = (1 << 11),  // Instruction cache reset
    FLASH_ACR_DCRST   = (1 << 12)   // Data cache reset
};

// Flash status register (FLASH_SR)
enum FLASH_SR_Bits {
    FLASH_SR_EOP      = (1 << 0),   // End of operation
    FLASH_SR_OPERR    = (1 << 1),   // Operation error
    FLASH_SR_WRPERR   = (1 << 4),   // Write protection error
    FLASH_SR_PGAERR   = (1 << 5),   // Programming alignment error
    FLASH_SR_PGPERR   = (1 << 6),   // Programming parallelism error
    FLASH_SR_PGSERR   = (1 << 7),   // Programming sequence error
    FLASH_SR_RDERR    = (1 << 8),   // Read protection error (PCROP)
    FLASH_SR_BSY      = (1 << 16)   // Busy
};

// Flash control register (FLASH_CR)
enum FLASH_CR_Bits {
    FLASH_CR_PG       = (1 << 0),   // Programming
    FLASH_CR_SER      = (1 << 1),   // Sector erase
    FLASH_CR_MER      = (1 << 2),   // Mass erase
    
    // Sector number (bits 3-6)
    FLASH_CR_SNB_BIT  = 3,
    FLASH_CR_SNB_MASK = (0xF << FLASH_CR_SNB_BIT),
    
    // Program size
    FLASH_CR_PSIZE_BYTE   = (0 << 8),
    FLASH_CR_PSIZE_HWORD  = (1 << 8),
    FLASH_CR_PSIZE_WORD   = (2 << 8),
    FLASH_CR_PSIZE_DWORD  = (3 << 8),
    FLASH_CR_PSIZE_MASK   = (3 << 8),
    
    FLASH_CR_STRT     = (1 << 16),  // Start
    FLASH_CR_EOPIE    = (1 << 24),  // End of operation interrupt enable
    FLASH_CR_ERRIE    = (1 << 25),  // Error interrupt enable
    FLASH_CR_LOCK     = (1 << 31)   // Lock
};

// Flash option control register (FLASH_OPTCR)
enum FLASH_OPTCR_Bits {
    FLASH_OPTCR_OPTLOCK   = (1 << 0),   // Option lock
    FLASH_OPTCR_OPTSTRT   = (1 << 1),   // Option start
    
    // BOR level
    FLASH_OPTCR_BOR_LEV1  = (0 << 2),   // BOR Level 1 (2.7-3.6V)
    FLASH_OPTCR_BOR_LEV2  = (1 << 2),   // BOR Level 2 (2.4-2.7V)
    FLASH_OPTCR_BOR_LEV3  = (2 << 2),   // BOR Level 3 (2.1-2.4V)
    FLASH_OPTCR_BOR_OFF   = (3 << 2),   // BOR off
    FLASH_OPTCR_BOR_MASK  = (3 << 2),
    
    FLASH_OPTCR_WDG_SW    = (1 << 5),   // Watchdog software
    FLASH_OPTCR_nRST_STOP = (1 << 6),   // No reset on stop
    FLASH_OPTCR_nRST_STDBY = (1 << 7),  // No reset on standby
    
    // Read protection level
    FLASH_OPTCR_RDP_BIT   = 8,
    FLASH_OPTCR_RDP_MASK  = (0xFF << FLASH_OPTCR_RDP_BIT),
    FLASH_OPTCR_RDP_LEVEL0 = (0xAA << FLASH_OPTCR_RDP_BIT),
    FLASH_OPTCR_RDP_LEVEL1 = (0x00 << FLASH_OPTCR_RDP_BIT),
    FLASH_OPTCR_RDP_LEVEL2 = (0xCC << FLASH_OPTCR_RDP_BIT),
    
    // Write protection
    FLASH_OPTCR_nWRP_BIT  = 16,
    FLASH_OPTCR_nWRP_MASK = (0xFFF << FLASH_OPTCR_nWRP_BIT)
};

// Flash key register values
enum FLASH_Keys {
    FLASH_KEY1 = 0x45670123,
    FLASH_KEY2 = 0xCDEF89AB,
    FLASH_OPTKEY1 = 0x08192A3B,
    FLASH_OPTKEY2 = 0x4C5D6E7F
};

// Helper macros for sector selection
#define FLASH_CR_SNB(sector) (((sector) << FLASH_CR_SNB_BIT) & FLASH_CR_SNB_MASK)

void FLASH_SetLatency(uint32_t latency);
void FLASH_Unlock(void);
void FLASH_Lock(void);