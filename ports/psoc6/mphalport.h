#ifndef MICROPY_INCLUDED_PSOC6_MPHALPORT_H
#define MICROPY_INCLUDED_PSOC6_MPHALPORT_H

#include "py/mpconfig.h"
#include "py/ringbuf.h"
#include "py/obj.h"
//#include "py/mphal.h"
#include "drivers/psoc6_gpio.h"

#define mp_hal_pin_t cyhal_gpio_t
#define mp_hal_pin_dir_t cyhal_gpio_direction_t
#define mp_hal_pin_drive_mode_t cyhal_gpio_drive_mode_t
#define mp_hal_pin_rslt_t cy_rslt_t 

extern int mp_interrupt_char;

/*static inline mp_uint_t mp_hal_ticks_ms(void) {
    return 0;
}
static inline void mp_hal_set_interrupt_char(char c) {
}*/

void mp_hal_set_interrupt_char(int c);

static inline unsigned int mp_hal_pin_name(mp_hal_pin_t pin) 
{
    return pin;
}

static inline mp_hal_pin_rslt_t mp_hal_pin_init(mp_hal_pin_t pin, mp_hal_pin_dir_t dir, mp_hal_pin_drive_mode_t drive_mode, bool init_val) 
{
    return gpio_init(pin, dir, drive_mode, init_val);
}

static inline void mp_hal_pin_toggle_onBoardLed()
{
    return gpio_toggle_onBoardLed();
}

static inline void mp_hal_pin_toggle(mp_hal_pin_t pin)
{
    return gpio_toggle(pin);
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



/*static inline int mp_hal_pin_read(mp_hal_pin_t pin) {
    return gpio_read(pin);
}

static inline void mp_hal_pin_write(mp_hal_pin_t pin, int v) {
    return gpio_write(pin, v);
}

static inline void mp_hal_pin_low(mp_hal_pin_t pin) {
    return gpio_write(pin, 0);
}

static inline void mp_hal_pin_high(mp_hal_pin_t pin) {
    return gpio_write(pin, 1);
}

static inline void mp_hal_pin_output(mp_hal_pin_obj_t pin) {
    //gpio_set_pin_direction(pin, GPIO_DIRECTION_OUT);
}*/

#endif // MICROPY_INCLUDED_PSOC6_MPHALPORT_H