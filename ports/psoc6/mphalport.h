#ifndef MICROPY_INCLUDED_PSOC6_MPHALPORT_H
#define MICROPY_INCLUDED_PSOC6_MPHALPORT_H

//mpy includes
#include "py/mpconfig.h"
#include "py/ringbuf.h"
#include "py/obj.h"
#include "py/runtime.h"
// #include "py/mphal.h"

//std includes
#include <stdio.h>
#include <stdlib.h>

//cy includes
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


// Private methods (?)
// API to return clock freq; Fast CLK (CM4) is the main sys clk
static inline mp_uint_t mp_hal_get_cpu_freq(void) {
//    return Cy_SysClk_ClkPathMuxGetFrequency(Cy_SysClk_ClkPathGetSource(0UL));
    return Cy_SysClk_ClkFastGetFrequency();
}

// API to return clock freq divider for Fast CLK (CM4)
static inline mp_uint_t mp_hal_get_cpu_freq_div(void) {
    return Cy_SysClk_ClkFastGetDivider();
}

// //function to return Pin.Mode
// static inline int mp_hal_get_pin_mode(uint32_t pin){
//     return gpio_get_mode(uint32_t pin);
// }

//PSoC6 HAL functions


#endif // MICROPY_INCLUDED_PSOC6_MPHALPORT_H
