#ifndef MICROPY_INCLUDED_PSOC6_MPHALPORT_H
#define MICROPY_INCLUDED_PSOC6_MPHALPORT_H

// mpy includes
#include "py/mpconfig.h"
#include "py/ringbuf.h"
#include "py/obj.h"
#include "py/runtime.h"
// #include "py/mphal.h"

// std includes
#include <stdio.h>
#include <stdlib.h>

// cy includes
#include "cy_sysclk.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_pdl.h"

extern int mp_interrupt_char;

/*static inline mp_uint_t mp_hal_ticks_ms(void) {
    return 0;
}
static inline void mp_hal_set_interrupt_char(char c) {
}*/

void mp_hal_set_interrupt_char(int c);

#endif // MICROPY_INCLUDED_PSOC6_MPHALPORT_H
