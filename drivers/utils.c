#include "utils.h"

void interrupts_on(void) {
    asm volatile ("cpsie i");
}

void interrupts_off(void) {
    asm volatile ("cpsid i");
}