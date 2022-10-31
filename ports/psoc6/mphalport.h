#ifndef MICROPY_INCLUDED_PSOC6_MPHALPORT_H
#define MICROPY_INCLUDED_PSOC6_MPHALPORT_H

#include "py/mpconfig.h"
#include "py/ringbuf.h"
#include "py/obj.h"
//#include "py/mphal.h"
#include "drivers/psoc6_gpio.h"

#define mp_hal_pin_obj_t cyhal_gpio_t

extern int mp_interrupt_char;

/*static inline mp_uint_t mp_hal_ticks_ms(void) {
    return 0;
}
static inline void mp_hal_set_interrupt_char(char c) {
}*/

void mp_hal_set_interrupt_char(int c);

static inline unsigned int mp_hal_pin_name(mp_hal_pin_obj_t pin) {
    return pin;
}

/*static inline void mp_hal_pin_input(mp_hal_pin_obj_t pin) {
    gpio_set_function(pin, GPIO_FUNC_SIO);
    gpio_set_dir(pin, GPIO_IN);
    machine_pin_open_drain_mask &= ~(1 << pin);
}

static inline void mp_hal_pin_output(mp_hal_pin_obj_t pin) {
    cyhal_gpio_configure(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode);
    return;
}*/

static inline int mp_hal_pin_read(mp_hal_pin_obj_t pin) {
    return gpio_read(pin);
}

static inline void mp_hal_pin_write(mp_hal_pin_obj_t pin, int v) {
    gpio_write(pin, v);
}

static inline void mp_hal_pin_low(mp_hal_pin_obj_t pin) {
    gpio_write(pin, 0);
}

static inline void mp_hal_pin_high(mp_hal_pin_obj_t pin) {
    gpio_write(pin, 1);
}

static inline void mp_hal_pin_toggle_led(mp_hal_pin_obj_t pin){
    gpio_toggle_led();
}

#endif // MICROPY_INCLUDED_PSOC6_MPHALPORT_H