#ifndef MICROPY_INCLUDED_PSOC6_PIN_H
#define MICROPY_INCLUDED_PSOC6_PIN_H

//cy includes
#include "cybsp.h"
#include "cyhal.h"
#include "cy_pdl.h"

//std includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//Note: Used to define gpio value QSTRs 
//Note: not in MPY guidelines but may needed later for board-specific active-low/active-high pins
#define GPIO_STATE_ON                   (0U)
#define GPIO_STATE_OFF                  (1U)

#define gpio_init_success               CY_RSLT_SUCCESS
#define gpio_init_rslt                  cy_rslt_t
#define gpio_pin_t                      cyhal_gpio_t
#define gpio_pin_dir_t                  cyhal_gpio_direction_t
#define gpio_pin_drive_mode_t           cyhal_gpio_drive_mode_t

gpio_init_rslt gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode, bool init_val);

/* !Placeholder : Add Other GPIO functions here  */

//function to get HSIOM config of a pin
en_hsiom_sel_t PIN_GET_HSIOM_FUNC(uint32_t pin);

//function to get cypdl drive modes, correlated to cyhal drive modes in file: cyhal_gpio.c
uint32_t GPIO_GET_CYPDL_DRIVE(uint32_t pin);

//function to check if pin is in mode Pin.OPEN_DRAIN.
//drive comparisons done with PDL drive modes since function is from PDL (not HAL)
bool GPIO_IS_OPEN_DRAIN(uint32_t pin);

//function to check if pin is in mode Pin.OUT; TODO: can be also implemented by checking input buffer on/off
bool GPIO_IS_OUT(uint32_t pin);

//function to check if pin is in mode Pin.IN; TODO: can be also implemented by checking input buffer on/off
bool GPIO_IS_IN(uint32_t pin);
    
//function to check if pin has pull Pin.PULL_UP
bool GPIO_IS_PULL_UP(uint32_t pin);

//function to check if pin has pull Pin.PULL_DOWN
bool GPIO_IS_PULL_DOWN(uint32_t pin);

//function to check Pin.value
uint8_t GPIO_GET_VALUE(uint32_t pin);
    
//function to call Pin.value only for pins with mode Pin.IN; used for __call__ function
//uses mp_const_none type for None/undefined return
int8_t GPIO_GET_VALUE_CALL(uint32_t pin);

//function to set Pin.value to 1; sets the output buffer which drives the output driver
void GPIO_SET_VALUE(uint32_t pin);

//function to set Pin.value to 0; clear the output buffer which drives the output driver
void GPIO_CLR_VALUE(uint32_t pin);

//function to toggle Pin.value; toggle the output buffer which drives the output driver
void GPIO_TOGGLE_VALUE(uint32_t pin);

#endif // MICROPY_INCLUDED_PSOC6_PIN_H
