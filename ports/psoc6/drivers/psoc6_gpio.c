/* Wrapper for CYHAL GPIO functions */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "psoc6_gpio.h"

cy_rslt_t gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode, bool init_val) {
    return cyhal_gpio_init(pin, direction, drive_mode, init_val);
}

void gpio_toggle_onBoardLed() {
    gpio_init(P13_7, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    cyhal_gpio_toggle(P13_7);
    return;
}

void gpio_toggle(cyhal_gpio_t pin) {
    return cyhal_gpio_toggle(pin);
}

// int gpio_get_mode(uint32_t pin){
//     if(gpio_get_cypdl_drive(pin) == CY_GPIO_DM_HIGHZ)
//         return GPIO_MODE_IN;
//     else if (gpio_get_cypdl_drive(pin) == CY_GPIO_DM_STRONG_IN_OFF) //pin cfgd as o/p drive so Input buffer is off.
//         return GPIO_MODE_OUT;
//     else if (gpio_get_cypdl_drive(pin) == CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW)
//         return GPIO_MODE_OPEN_DRAIN; 
// }


// //function to get cypdl drive modes, correlated to cyhal drive modes in file: cyhal_gpio.c
// uint32_t gpio_get_cypdl_drive(uint32_t pin){
//     return Cy_GPIO_GetDrivemode(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin));
// }