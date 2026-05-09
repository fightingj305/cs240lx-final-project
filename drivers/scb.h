#pragma once
#include <stdint.h>

#define SCB ((SCB_Regs *)0xE000ED00U)

typedef struct {
    volatile uint32_t CPUID;   // CPUID Base Register
    volatile uint32_t ICSR;    // Interrupt Control and State Register
    volatile uint32_t VTOR;    // Vector Table Offset Register
    volatile uint32_t AIRCR;   // Application Interrupt and Reset Control Register
    volatile uint32_t SCR;     // System Control Register
    volatile uint32_t CCR;     // Configuration and Control Register
    volatile uint32_t SHP[3];  // System Handler Priority Registers
    volatile uint32_t SHCSR;   // System Handler Control and State Register
    volatile uint32_t CFSR;    // Configurable Fault Status Register
    volatile uint32_t HFSR;    // HardFault Status Register
    volatile uint32_t DFSR;    // Debug Fault Status Register
    volatile uint32_t MMFAR;   // MemManage Fault Address Register
    volatile uint32_t BFAR;    // BusFault Address Register
    volatile uint32_t AFSR;    // Auxiliary Fault Status Register
    volatile uint32_t PFR[2];  // Processor Feature Register
    volatile uint32_t DFR;     // Debug Feature Register
    volatile uint32_t ADR;     // Auxiliary Feature Register
    volatile uint32_t MMFR[4]; // Memory Model Feature Register
    volatile uint32_t ISAR[5]; // Instruction Set Attributes Register
    uint32_t RESERVED0[5];
    volatile uint32_t CPACR;   // Coprocessor Access Control Register
} SCB_Regs;

// CPUID Base Register (SCB_CPUID) - Read only
enum SCB_CPUID_Bits {
    SCB_CPUID_REVISION_MASK     = (0xF << 0),
    SCB_CPUID_PARTNO_MASK       = (0xFFF << 4),
    SCB_CPUID_ARCHITECTURE_MASK = (0xF << 16),
    SCB_CPUID_VARIANT_MASK      = (0xF << 20),
    SCB_CPUID_IMPLEMENTER_MASK  = (0xFF << 24)
};

// Interrupt Control and State Register (SCB_ICSR)
enum SCB_ICSR_Bits {
    SCB_ICSR_VECTACTIVE_MASK = (0x1FF << 0),  // Active exception number
    SCB_ICSR_RETTOBASE       = (1 << 11),     // Return to base level
    SCB_ICSR_VECTPENDING_MASK = (0x1FF << 12), // Pending exception number
    SCB_ICSR_ISRPENDING      = (1 << 22),     // Interrupt pending flag
    SCB_ICSR_PENDSTCLR       = (1 << 25),     // SysTick clear-pending bit
    SCB_ICSR_PENDSTSET       = (1 << 26),     // SysTick set-pending bit
    SCB_ICSR_PENDSVCLR       = (1 << 27),     // PendSV clear-pending bit
    SCB_ICSR_PENDSVSET       = (1 << 28),     // PendSV set-pending bit
    SCB_ICSR_NMIPENDSET      = (1 << 31)      // NMI set-pending bit
};

// Vector Table Offset Register (SCB_VTOR)
enum SCB_VTOR_Bits {
    SCB_VTOR_TBLOFF_MASK = (0x1FFFFFF << 7)   // Vector table base offset
};

// Application Interrupt and Reset Control Register (SCB_AIRCR)
enum SCB_AIRCR_Bits {
    SCB_AIRCR_VECTRESET      = (1 << 0),      // Reserved for debug use
    SCB_AIRCR_VECTCLRACTIVE  = (1 << 1),      // Reserved for debug use
    SCB_AIRCR_SYSRESETREQ    = (1 << 2),      // System reset request
    
    // Priority grouping
    SCB_AIRCR_PRIGROUP_BIT   = 8,
    SCB_AIRCR_PRIGROUP_MASK  = (0x7 << SCB_AIRCR_PRIGROUP_BIT),
    SCB_AIRCR_PRIGROUP_0_7   = (7 << SCB_AIRCR_PRIGROUP_BIT), // 0 bits group, 7 bits subgroup
    SCB_AIRCR_PRIGROUP_1_6   = (6 << SCB_AIRCR_PRIGROUP_BIT), // 1 bits group, 6 bits subgroup
    SCB_AIRCR_PRIGROUP_2_5   = (5 << SCB_AIRCR_PRIGROUP_BIT), // 2 bits group, 5 bits subgroup
    SCB_AIRCR_PRIGROUP_3_4   = (4 << SCB_AIRCR_PRIGROUP_BIT), // 3 bits group, 4 bits subgroup
    SCB_AIRCR_PRIGROUP_4_3   = (3 << SCB_AIRCR_PRIGROUP_BIT), // 4 bits group, 3 bits subgroup
    SCB_AIRCR_PRIGROUP_5_2   = (2 << SCB_AIRCR_PRIGROUP_BIT), // 5 bits group, 2 bits subgroup
    SCB_AIRCR_PRIGROUP_6_1   = (1 << SCB_AIRCR_PRIGROUP_BIT), // 6 bits group, 1 bits subgroup
    SCB_AIRCR_PRIGROUP_7_0   = (0 << SCB_AIRCR_PRIGROUP_BIT), // 7 bits group, 0 bits subgroup
    
    SCB_AIRCR_ENDIANNESS     = (1 << 15),     // Data endianness (RO)
    
    // Vector key
    SCB_AIRCR_VECTKEY_BIT    = 16,
    SCB_AIRCR_VECTKEY_MASK   = (0xFFFF << SCB_AIRCR_VECTKEY_BIT),
    SCB_AIRCR_VECTKEY        = (0x05FA << SCB_AIRCR_VECTKEY_BIT)
};

// System Control Register (SCB_SCR)
enum SCB_SCR_Bits {
    SCB_SCR_SLEEPONEXIT = (1 << 1),  // Sleep-on-exit when returning from Handler to Thread
    SCB_SCR_SLEEPDEEP   = (1 << 2),  // Sleep deep bit
    SCB_SCR_SEVONPEND   = (1 << 4)   // Send Event on Pending bit
};

// Configuration and Control Register (SCB_CCR)
enum SCB_CCR_Bits {
    SCB_CCR_NONBASETHRDENA = (1 << 0),  // Non base thread enable
    SCB_CCR_USERSETMPEND   = (1 << 1),  // User set PEND enable
    SCB_CCR_UNALIGN_TRP    = (1 << 3),  // Unaligned access trap enable
    SCB_CCR_DIV_0_TRP      = (1 << 4),  // Divide by zero trap enable
    SCB_CCR_BFHFNMIGN      = (1 << 8),  // Ignore data bus fault during HardFault/NMI
    SCB_CCR_STKALIGN       = (1 << 9)   // Stack alignment on exception entry
};

// System Handler Control and State Register (SCB_SHCSR)
enum SCB_SHCSR_Bits {
    SCB_SHCSR_MEMFAULTACT    = (1 << 0),   // MemManage fault active
    SCB_SHCSR_BUSFAULTACT    = (1 << 1),   // BusFault active
    SCB_SHCSR_USGFAULTACT    = (1 << 3),   // UsageFault active
    SCB_SHCSR_SVCALLACT      = (1 << 7),   // SVCall active
    SCB_SHCSR_MONITORACT     = (1 << 8),   // Monitor active
    SCB_SHCSR_PENDSVACT      = (1 << 10),  // PendSV active
    SCB_SHCSR_SYSTICKACT     = (1 << 11),  // SysTick active
    SCB_SHCSR_USGFAULTPENDED = (1 << 12),  // UsageFault pending
    SCB_SHCSR_MEMFAULTPENDED = (1 << 13),  // MemManage fault pending
    SCB_SHCSR_BUSFAULTPENDED = (1 << 14),  // BusFault pending
    SCB_SHCSR_SVCALLPENDED   = (1 << 15),  // SVCall pending
    SCB_SHCSR_MEMFAULTENA    = (1 << 16),  // MemManage fault enable
    SCB_SHCSR_BUSFAULTENA    = (1 << 17),  // BusFault enable
    SCB_SHCSR_USGFAULTENA    = (1 << 18)   // UsageFault enable
};

// Configurable Fault Status Register (SCB_CFSR)
// Contains MMFSR, BFSR, and UFSR
enum SCB_CFSR_Bits {
    // MemManage Fault Status Register (MMFSR) - bits [7:0]
    SCB_CFSR_IACCVIOL    = (1 << 0),   // Instruction access violation
    SCB_CFSR_DACCVIOL    = (1 << 1),   // Data access violation
    SCB_CFSR_MUNSTKERR   = (1 << 3),   // MemManage fault on unstacking
    SCB_CFSR_MSTKERR     = (1 << 4),   // MemManage fault on stacking
    SCB_CFSR_MLSPERR     = (1 << 5),   // MemManage fault during FP lazy state preservation
    SCB_CFSR_MMARVALID   = (1 << 7),   // MMFAR valid flag
    
    // BusFault Status Register (BFSR) - bits [15:8]
    SCB_CFSR_IBUSERR     = (1 << 8),   // Instruction bus error
    SCB_CFSR_PRECISERR   = (1 << 9),   // Precise data bus error
    SCB_CFSR_IMPRECISERR = (1 << 10),  // Imprecise data bus error
    SCB_CFSR_UNSTKERR    = (1 << 11),  // BusFault on unstacking
    SCB_CFSR_STKERR      = (1 << 12),  // BusFault on stacking
    SCB_CFSR_LSPERR      = (1 << 13),  // BusFault during FP lazy state preservation
    SCB_CFSR_BFARVALID   = (1 << 15),  // BFAR valid flag
    
    // UsageFault Status Register (UFSR) - bits [31:16]
    SCB_CFSR_UNDEFINSTR  = (1 << 16),  // Undefined instruction
    SCB_CFSR_INVSTATE    = (1 << 17),  // Invalid state
    SCB_CFSR_INVPC       = (1 << 18),  // Invalid PC
    SCB_CFSR_NOCP        = (1 << 19),  // No coprocessor
    SCB_CFSR_UNALIGNED   = (1 << 24),  // Unaligned access
    SCB_CFSR_DIVBYZERO   = (1 << 25)   // Divide by zero
};

// HardFault Status Register (SCB_HFSR)
enum SCB_HFSR_Bits {
    SCB_HFSR_VECTTBL  = (1 << 1),   // Vector table read fault
    SCB_HFSR_FORCED   = (1 << 30),  // Forced HardFault
    SCB_HFSR_DEBUGEVT = (1 << 31)   // Debug event
};

// Debug Fault Status Register (SCB_DFSR)
enum SCB_DFSR_Bits {
    SCB_DFSR_HALTED   = (1 << 0),  // Halt request
    SCB_DFSR_BKPT     = (1 << 1),  // Breakpoint
    SCB_DFSR_DWTTRAP  = (1 << 2),  // DWT match
    SCB_DFSR_VCATCH   = (1 << 3),  // Vector catch
    SCB_DFSR_EXTERNAL = (1 << 4)   // External debug request
};

// Coprocessor Access Control Register (SCB_CPACR)
enum SCB_CPACR_Bits {
    // CP10 (Floating Point)
    SCB_CPACR_CP10_DENIED    = (0 << 20),
    SCB_CPACR_CP10_PRIVILEGED = (1 << 20),
    SCB_CPACR_CP10_FULL      = (3 << 20),
    SCB_CPACR_CP10_MASK      = (3 << 20),
    
    // CP11 (Floating Point)
    SCB_CPACR_CP11_DENIED    = (0 << 22),
    SCB_CPACR_CP11_PRIVILEGED = (1 << 22),
    SCB_CPACR_CP11_FULL      = (3 << 22),
    SCB_CPACR_CP11_MASK      = (3 << 22)
};

// System Handler Priority Register indexes
enum SCB_SHP_Index {
    SCB_SHP_MEMMANAGE  = 0,  // SHP[0] bits [7:0]   - MemManage priority
    SCB_SHP_BUSFAULT   = 1,  // SHP[0] bits [15:8]  - BusFault priority
    SCB_SHP_USAGEFAULT = 2,  // SHP[0] bits [23:16] - UsageFault priority
    SCB_SHP_SVCALL     = 7,  // SHP[1] bits [31:24] - SVCall priority
    SCB_SHP_PENDSV     = 10, // SHP[2] bits [23:16] - PendSV priority
    SCB_SHP_SYSTICK    = 11  // SHP[2] bits [31:24] - SysTick priority
};