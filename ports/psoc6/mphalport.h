#ifndef MICROPY_INCLUDED_PSOC6_MPHALPORT_H
#define MICROPY_INCLUDED_PSOC6_MPHALPORT_H

//std includes
#include <stdbool.h>

// mpy includes
#include "py/mpconfig.h"

extern int mp_interrupt_char;

static inline mp_uint_t mp_hal_ticks_ms(void) {
    return false;
}

/*Note: Generally, the functions in the mp_* files are defined as static functions by the community,
probably to keep their scope limited to the file and prevent miscompilation for other ports. The below
function is an example of that. However, since it is later defined in a mphalport.c file, it cannot be
a static function. Hence it was later declared as a simple (non-static) function. The question would be
whether to keep a mphalport.c file in the end (if these function implementations are pretty small). */
/*
static inline void mp_hal_set_interrupt_char(char c) {
}*/

void mp_hal_set_interrupt_char(int c);

#endif // MICROPY_INCLUDED_PSOC6_MPHALPORT_H
