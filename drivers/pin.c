#include "pin.h"
#include "rcc.h"
#include "utils.h"

void Pin_Clock_Enable(const Pin *pin) {
    switch ((uint32_t)pin->port) {
        case (uint32_t)GPIOA:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOA_EN;
            break;
        case (uint32_t)GPIOB:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOB_EN;
            break;
        case (uint32_t)GPIOC:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOC_EN;
            break;
        case (uint32_t)GPIOD:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOD_EN;
            break;
        case (uint32_t)GPIOE:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOE_EN;
            break;
        case (uint32_t)GPIOH:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOH_EN;
            break;
        default:
            PANIC("Invalid GPIO port");
            break;
    }
}

void Pin_Config(const Pin *pin, Pin_Mode mode, Pin_OutputType ot, Pin_Speed speed, Pin_Pull pull) {
    ASSERT(pin->pin_num < 16);

    Pin_Clock_Enable(pin);

    uint32_t current_mode = pin->port->MODER;
    current_mode &= ~(0x3 << (pin->pin_num * 2));
    pin->port->MODER = current_mode | (mode << (pin->pin_num * 2));

    uint32_t current_ot = pin->port->OTYPER;
    current_ot &= ~(0x1 << pin->pin_num);
    pin->port->OTYPER = current_ot | (ot << pin->pin_num);

    uint32_t current_ospeed = pin->port->OSPEEDR;
    current_ospeed &= ~(0x3 << (pin->pin_num * 2));
    pin->port->OSPEEDR = current_ospeed | (speed << (pin->pin_num * 2));

    uint32_t current_pupdr = pin->port->PUPDR;
    current_pupdr &= ~(0x3 << (pin->pin_num * 2));
    pin->port->PUPDR = current_pupdr | (pull << (pin->pin_num * 2));
}

void Pin_Config_AF(const Pin *pin, Pin_AF af, Pin_OutputType ot, Pin_Speed speed, Pin_Pull pull) {
    ASSERT(pin->pin_num < 16);
    
    Pin_Clock_Enable(pin);

    uint32_t current_mode = pin->port->MODER;
    current_mode &= ~(0x3 << (pin->pin_num * 2));
    pin->port->MODER = current_mode | (PIN_MODE_AF << (pin->pin_num * 2));

    uint32_t current_afr = pin->pin_num < 8 ? pin->port->AFRL : pin->port->AFRH;
    current_afr &= ~(0xF << ((pin->pin_num % 8) * 4));
    if (pin->pin_num < 8) {
        pin->port->AFRL = current_afr | (af << ((pin->pin_num % 8) * 4));
    } else {
        pin->port->AFRH = current_afr | (af << ((pin->pin_num % 8) * 4));
    }

    uint32_t current_ot = pin->port->OTYPER;
    current_ot &= ~(0x1 << pin->pin_num);
    pin->port->OTYPER = current_ot | (ot << pin->pin_num);

    uint32_t current_ospeed = pin->port->OSPEEDR;
    current_ospeed &= ~(0x3 << (pin->pin_num * 2));
    pin->port->OSPEEDR = current_ospeed | (speed << (pin->pin_num * 2));

    uint32_t current_pupdr = pin->port->PUPDR;
    current_pupdr &= ~(0x3 << (pin->pin_num * 2));
    pin->port->PUPDR = current_pupdr | (pull << (pin->pin_num * 2));
}
