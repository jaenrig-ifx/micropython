/* Wrapper for CYHAL GPIO functions */

//std includes
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

//function to get HSIOM config of a pin
en_hsiom_sel_t PIN_GET_HSIOM_FUNC(uint32_t pin){
    return Cy_GPIO_GetHSIOM(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin));
}

//function to get cypdl drive modes, correlated to cyhal drive modes in file: cyhal_gpio.c
uint32_t GPIO_GET_CYPDL_DRIVE(uint32_t pin){
    return Cy_GPIO_GetDrivemode(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin));
}

//function to check if pin is in mode Pin.OPEN_DRAIN.
//drive comparisons done with PDL drive modes since function is from PDL (not HAL)
bool GPIO_IS_OPEN_DRAIN(uint32_t pin){
    if(GPIO_GET_CYPDL_DRIVE(pin) == CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW)
        return 1;
    else
        return 0;
}

//function to check if pin is in mode Pin.OUT; TODO: can be also implemented by checking input buffer on/off
bool GPIO_IS_OUT(uint32_t pin){
    if(GPIO_GET_CYPDL_DRIVE(pin) == CY_GPIO_DM_STRONG_IN_OFF){ //pin cfgd as o/p drive so Input buffer is off.
        return 1;
    }
    else{
        return 0;
    }
}

//function to check if pin is in mode Pin.IN; TODO: can be also implemented by checking input buffer on/off
bool GPIO_IS_IN(uint32_t pin){
    if(GPIO_GET_CYPDL_DRIVE(pin) == CY_GPIO_DM_HIGHZ)
        return 1;
    else
        return 0;
}


//function to check if pin has pull Pin.PULL_UP
bool GPIO_IS_PULL_UP(uint32_t pin){
    if(GPIO_GET_CYPDL_DRIVE(pin) == CY_GPIO_DM_PULLUP_IN_OFF || GPIO_GET_CYPDL_DRIVE(pin) == CY_GPIO_DM_PULLUP)
        return 1;
    else
        return 0;
}

//function to check if pin has pull Pin.PULL_DOWN
bool GPIO_IS_PULL_DOWN(uint32_t pin){
    if(GPIO_GET_CYPDL_DRIVE(pin) == CY_GPIO_DM_PULLDOWN_IN_OFF || GPIO_GET_CYPDL_DRIVE(pin) == CY_GPIO_DM_PULLDOWN)
        return 1;
    else
        return 0;
}

//function to check Pin.value
uint8_t GPIO_GET_VALUE(uint32_t pin){
    if(GPIO_IS_OUT(pin)) //if Pin.Mode is Pin.OUT, read output driver
        return Cy_GPIO_ReadOut(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin));
    else //if Pin.Mode is Pin.IN, read pin.
        return Cy_GPIO_Read(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin));
}

//function to call Pin.value only for pins with mode Pin.IN; used for __call__ function
//uses mp_const_none type for None/undefined return
int8_t GPIO_GET_VALUE_CALL(uint32_t pin){
    if(GPIO_IS_IN(pin)) //if Pin.Mode is Pin.IN, return current pin input value
        return Cy_GPIO_Read(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin));
    else if(GPIO_IS_OUT(pin)) //if Pin.Mode is Pin.OUT, then return is undefined
        return -1; //undefined
    else{ //Pin.Mode is Pin.OPEN_DRAIN
            if (Cy_GPIO_ReadOut(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin)) == 0) //if 0 is driven in open_drain, then undefined
                return -1; //undefined
            else
                return Cy_GPIO_Read(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin));
        }
}

//function to set Pin.value to 1; sets the output buffer which drives the output driver
void GPIO_SET_VALUE(uint32_t pin){
    return Cy_GPIO_Set(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin));
}

//function to set Pin.value to 0; clear the output buffer which drives the output driver
void GPIO_CLR_VALUE(uint32_t pin){
    return Cy_GPIO_Clr(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin));
}

//function to toggle Pin.value; toggle the output buffer which drives the output driver
void GPIO_TOGGLE_VALUE(uint32_t pin){
    return Cy_GPIO_Inv(CYHAL_GET_PORTADDR(pin),CYHAL_GET_PIN(pin));
}

//function to return 64-bit silicon ID of given PSoC microcontroller
// A combined 64-bit unique ID. [63:57] - DIE_YEAR [56:56] - DIE_MINOR [55:48] - DIE_SORT [47:40] - DIE_Y [39:32] - DIE_X [31:24] - DIE_WAFER [23:16] - DIE_LOT[2] [15: 8] - DIE_LOT[1] [ 7: 0] - DIE_LOT[0]
uint64_t CYPDL_GET_UNIQUE_ID(void){
    return Cy_SysLib_GetUniqueId();
}

//using watchdog timer to count to minimum value (1ms) to trigger reset
//thread-safe way as other methods might interfere with pending interrupts, threads etc.
void CYHAL_RESET(void){
    cyhal_wdt_t wdt_obj;
    cyhal_wdt_init(&wdt_obj, 1); //min 1ms count time
    cyhal_wdt_start(&wdt_obj);
}

//get reset cause of the last system reset
//macros defined here: cy_syslib.h
uint32_t CYPDL_RESET_CAUSE(void){
    return Cy_SysLib_GetResetReason();
}

//helper function to generate random alpha-numeric hash
uint8_t mp_rand_hash(uint8_t length) {
    uint8_t hash_sum=0;
    char charset[] = {'0','1','2','3','4','5','6','7','8','9'}; //hash can be made stronger but
                                                                // uint8_t can only hold <=255

    while (length-- > 0) {
        uint8_t idx = rand() % sizeof(charset);
        hash_sum = hash_sum + (int)charset[idx];
    }
    return hash_sum;
}

//function to disable global IRQs
//returns alpha-numeric hash to enable IRQs later
//see: https://docs.zephyrproject.org/apidoc/latest/group__isr__apis.html#ga19fdde73c3b02fcca6cf1d1e67631228
uint8_t CY_DISABLE_GLOBAL_IRQ(void){
   uint8_t state = mp_rand_hash(10); //10 chars long key gen;
    __disable_irq();
    irq_key = state;
    return state;
}

//function to enable global IRQs
//uses passed alpha-numeric key to verify and enable IRQs
bool CY_ENABLE_GLOBAL_IRQ(uint8_t state){
    if (state == irq_key){
        __enable_irq();
        return 1;
    }
    else
        return 0;
}