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
      printf("PANIC: %s at %s: %s\n", msg, __FILE__, __LINE__); \
        while(1); \
    } while(0)

__attribute__((naked))
void PUT32(uint32_t location, uint32_t value);

__attribute__((naked))
uint32_t GET32(uint32_t location);

void put32(volatile uint32_t *addr, uint32_t value);

uint32_t get32(volatile uint32_t *addr);

void RMW_OR(uint32_t reg, uint32_t mask);
void RMW_AND(uint32_t reg, uint32_t mask);

void interrupts_on(void);
void interrupts_off(void);
