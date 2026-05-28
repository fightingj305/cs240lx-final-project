#pragma once
#include <stdint.h>
#include "pin.h"

#define SYSCFG ((SYSCFG_Regs *)0x40013800U)

typedef struct {
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
    uint32_t reserved[2];
    volatile uint32_t CMPCR;
} SYSCFG_Regs;

static inline void SYSCFG_EXTICR_Set(Pin *pin) {
    uint32_t shamt = ((pin->pin_num & 0x3) << 2);
    uint32_t port = ((uint32_t)pin->port - (uint32_t)GPIOA) >> 10;
    SYSCFG->EXTICR[(pin->pin_num >> 2)] &= ~(0xF << shamt);
    SYSCFG->EXTICR[(pin->pin_num >> 2)] |= port << shamt;
}