#ifndef MICROPY_INCLUDED_PSOC6_PIN_H
#define MICROPY_INCLUDED_PSOC6_PIN_H
#include "cybsp.h"
#include "cyhal.h"

#define DEFAULT_LED_PIN 13

void gpio_init();
void gpio_configure();

void gpio_set_pin_input();
void gpio_set_pin_output();

void gpio_write();
bool gpio_read();

void gpio_toggle_led();

//void pin_enable_event();

#endif // MICROPY_INCLUDED_PSOC6_PIN_H