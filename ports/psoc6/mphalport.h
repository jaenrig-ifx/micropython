static inline mp_uint_t mp_hal_ticks_ms(void) {
    return 0;
}
static inline void mp_hal_set_interrupt_char(char c) {
}

#include "cy_sysclk.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_pdl.h"

//PSoC6 HAL functions

//API to return clock freq; Fast CLK (CM4) is the main sys clk
static inline mp_uint_t mp_hal_get_cpu_freq(void) {
//    return Cy_SysClk_ClkPathMuxGetFrequency(Cy_SysClk_ClkPathGetSource(0UL));	
    return Cy_SysClk_ClkFastGetFrequency();
}

//API to return clock freq divider for Fast CLK (CM4)
static inline mp_uint_t mp_hal_get_cpu_freq_div(void) {
    return Cy_SysClk_ClkFastGetDivider();
}

#define CY_PROTO_062_4343_GPIOs 2

// //GPIO array to map to CYHAL pins
// cyhal_gpio_t CY_GPIO_array[CY_PROTO_062_4343_GPIOs] = {
//     (P13_7), //GPIO 0 - LED
//     (P0_4),  //GPIO 1 - BTN
// };