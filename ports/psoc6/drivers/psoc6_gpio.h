#ifndef MICROPY_INCLUDED_PSOC6_PIN_H
#define MICROPY_INCLUDED_PSOC6_PIN_H

#include "cybsp.h"
#include "cyhal.h"

#define GPIO_STATE_ON                   (0U)
#define GPIO_STATE_OFF                  (1U)

cy_rslt_t gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode, bool init_val);
void gpio_toggle_onBoardLed();
void gpio_toggle(cyhal_gpio_t pin);

/* !Placeholder : Add Other GPIO functions here  */

#endif // MICROPY_INCLUDED_PSOC6_PIN_H