#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "psoc6_gpio.h"

void gpio_init()
{
    (void) cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, 
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    cyhal_gpio_toggle(CYBSP_USER_LED);
    //cyhal_gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode, bool init_val);
}

void gpio_write()
{
    //cyhal_gpio_write(cyhal_gpio_t pin, bool value);
    return;
}

bool gpio_read()
{
    return(cyhal_gpio_read(1));
}

void gpio_toggle_led()
{
    cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, 
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    cyhal_gpio_toggle(CYBSP_USER_LED);
    return;
}