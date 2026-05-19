#pragma once

#include <stdio.h>
#include <stdint.h>

#define ASSERT(x) \
    do { \
        if(!(x)) PANIC("Assertion failed: " #x); \
    } while(0)

#define DELAY(cycles) do { \
        for (volatile uint32_t i = (cycles); i > 0; i--); \
    } while(0)

#define PANIC(msg) do { \
      printf("PANIC: %s at %s: %d\n", msg, __FILE__, __LINE__); \
        while(1); \
    } while(0)

void interrupts_on(void);
void interrupts_off(void);
