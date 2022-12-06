#ifndef MICROPY_INCLUDED_PSOC6_PIN_H
#define MICROPY_INCLUDED_PSOC6_PIN_H

//cy includes
#include "cybsp.h"
#include "cyhal.h"

//std includes
#include <stdio.h>
#include <stdlib.h>

//Note: Used to define gpio value QSTRs 
//Note: not in MPY guidelines but may needed later for board-specific active-low/active-high pins
#define GPIO_STATE_ON                   (0U)
#define GPIO_STATE_OFF                  (1U)

#define gpio_init_success               CY_RSLT_SUCCESS
#define gpio_init_rslt                  cy_rslt_t
#define gpio_pin_t                      cyhal_gpio_t
#define gpio_pin_dir_t                  cyhal_gpio_direction_t
#define gpio_pin_drive_mode_t           cyhal_gpio_drive_mode_t

cy_rslt_t gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode, bool init_val);
void gpio_toggle_onBoardLed();
void gpio_toggle(cyhal_gpio_t pin);
//int  gpio_get_mode(uint32_t pin);
//uint32_t gpio_get_cypdl_drive(uint32_t pin);

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

//function to return 64-bit silicon ID of given PSoC microcontroller
// A combined 64-bit unique ID. [63:57] - DIE_YEAR [56:56] - DIE_MINOR [55:48] - DIE_SORT [47:40] - DIE_Y [39:32] - DIE_X [31:24] - DIE_WAFER [23:16] - DIE_LOT[2] [15: 8] - DIE_LOT[1] [ 7: 0] - DIE_LOT[0]
uint64_t CYPDL_GET_UNIQUE_ID(void);

//using watchdog timer to count to minimum value (1ms) to trigger reset
//thread-safe way as other methods might interfere with pending interrupts, threads etc.
void CYHAL_RESET(void);

//get reset cause of the last system reset
//macros defined here: cy_syslib.h
uint32_t CYPDL_RESET_CAUSE(void);

//global var to store current irq state/hash
extern uint8_t irq_key;

//helper function to generate random alpha-numeric hash
uint8_t mp_rand_hash(uint8_t length);

//function to disable global IRQs
//returns alpha-numeric hash to enable IRQs later
//see: https://docs.zephyrproject.org/apidoc/latest/group__isr__apis.html#ga19fdde73c3b02fcca6cf1d1e67631228
uint8_t CY_DISABLE_GLOBAL_IRQ(void);
//function to enable global IRQs
//uses passed alpha-numeric key to verify and enable IRQs
bool CY_ENABLE_GLOBAL_IRQ(uint8_t state);

#endif // MICROPY_INCLUDED_PSOC6_PIN_H
