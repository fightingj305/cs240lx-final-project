#pragma once
#include <stdint.h>
#include "pin.h"
#include "syscfg.h"

#define EXTI ((EXTI_Regs *)0x40013C00U)

typedef struct {
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_Regs;

enum EXTI_Line {
    EXTI_LINE0  = (1 << 0),
    EXTI_LINE1  = (1 << 1),
    EXTI_LINE2  = (1 << 2),
    EXTI_LINE3  = (1 << 3),
    EXTI_LINE4  = (1 << 4),
    EXTI_LINE5  = (1 << 5),
    EXTI_LINE6  = (1 << 6),
    EXTI_LINE7  = (1 << 7),
    EXTI_LINE8  = (1 << 8),
    EXTI_LINE9  = (1 << 9),
    EXTI_LINE10 = (1 << 10),
    EXTI_LINE11 = (1 << 11),
    EXTI_LINE12 = (1 << 12),
    EXTI_LINE13 = (1 << 13),
    EXTI_LINE14 = (1 << 14),
    EXTI_LINE15 = (1 << 15),
    EXTI_LINE16 = (1 << 16),
    EXTI_LINE17 = (1 << 17),
    EXTI_LINE18 = (1 << 18),
    EXTI_LINE21 = (1 << 21),
    EXTI_LINE22 = (1 << 22)
};

typedef enum {
    EXTI_TRIGGER_RISING  = 0x1,
    EXTI_TRIGGER_FALLING = 0x2,
    EXTI_TRIGGER_BOTH    = 0x3
} EXTI_Trigger;

void EXTI_Config(Pin *pin, EXTI_Trigger trigger);
static inline void EXTI_Disable(Pin *pin) {
    EXTI->IMR &= ~(1 << pin->pin_num);
}
static inline void EXTI_Clear_Pending(Pin *pin) {
    EXTI->PR |= (1 << pin->pin_num);
}
static inline bool EXTI_Check_Pending(Pin *pin) {
    return (EXTI->PR >> pin->pin_num) & 1;
}