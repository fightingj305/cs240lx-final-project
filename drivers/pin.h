#pragma once
#include <stdbool.h>
#include <stdint.h>

#define GPIOA  ((GPIO_Regs *) 0x40020000U)
#define GPIOB  ((GPIO_Regs *) 0x40020400U)
#define GPIOC  ((GPIO_Regs *) 0x40020800U)
#define GPIOD  ((GPIO_Regs *) 0x40020C00U)
#define GPIOE  ((GPIO_Regs *) 0x40021000U)
#define GPIOH  ((GPIO_Regs *) 0x40021C00U)

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} GPIO_Regs;

typedef enum {
  PIN_MODE_INPUT = 0x00,
  PIN_MODE_OUTPUT = 0x01,
  PIN_MODE_AF = 0x02,
  PIN_MODE_ANALOG = 0x03
} Pin_Mode;

typedef enum PinOutputType_t {
  PIN_OT_PUSH_PULL  = 0x00,
  PIN_OT_OPEN_DRAIN = 0x01
} Pin_OutputType;

typedef enum {
  PIN_SPEED_LOW = 0x00,
  PIN_SPEED_MEDIUM = 0x01,
  PIN_SPEED_FAST = 0x02,
  PIN_SPEED_HIGH = 0x03
} Pin_Speed;

typedef enum  {
  PIN_PULL_NONE = 0x00,
  PIN_PULL_UP = 0x01,
  PIN_PULL_DOWN = 0x02,
} Pin_Pull;

typedef enum {
    PIN_AF0,
    PIN_AF1,
    PIN_AF2,
    PIN_AF3,
    PIN_AF4,
    PIN_AF5,
    PIN_AF6,
    PIN_AF7,
    PIN_AF8,
    PIN_AF9,
    PIN_AF10,
    PIN_AF11,
    PIN_AF12,
    PIN_AF13,
    PIN_AF14,
    PIN_AF15,
} Pin_AF;

typedef struct {
    GPIO_Regs *port;
    uint8_t pin_num;
} Pin;

void Pin_Config(const Pin *pin, Pin_Mode mode, Pin_OutputType ot, Pin_Speed speed, Pin_Pull pull);
void Pin_Config_AF(const Pin *pin, Pin_AF af, Pin_OutputType ot, Pin_Speed speed, Pin_Pull pull);

inline void Pin_Set_High(const Pin *pin) {
    pin->port->BSRR = 1 << pin->pin_num;
}
inline void Pin_Set_Low(const Pin *pin) {
    pin->port->BSRR = 1 << (pin->pin_num + 16);
}
inline void Pin_Set(const Pin *pin, bool value) {
    value ? Pin_Set_High(pin) : Pin_Set_Low(pin);
}
inline bool Pin_Read(const Pin *pin) {
    return (pin->port->IDR >> pin->pin_num) & 1;
}