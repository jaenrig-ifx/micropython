/* Wrapper for CYHAL GPIO functions */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "psoc6_gpio.h"

cy_rslt_t gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode, bool init_val)
{
    return cyhal_gpio_init(pin, direction, drive_mode, init_val);
}

void gpio_toggle_onBoardLed()
{
    gpio_init(P13_7, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    cyhal_gpio_toggle(P13_7);
    return;
}

void gpio_toggle(cyhal_gpio_t pin)
{
    return cyhal_gpio_toggle(pin);
}
