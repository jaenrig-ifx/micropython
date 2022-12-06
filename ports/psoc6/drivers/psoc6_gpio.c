/* CYHAL GPIO functions */

#include "psoc6_gpio.h"

cy_rslt_t gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode, bool init_val) {
    return cyhal_gpio_init(pin, direction, drive_mode, init_val);
}

// function can be used in association with Pin.Mode() on MPY side
// int gpio_get_mode(uint32_t pin){
//     if(gpio_get_cypdl_drive(pin) == CY_GPIO_DM_HIGHZ)
//         return GPIO_MODE_IN;
//     else if (gpio_get_cypdl_drive(pin) == CY_GPIO_DM_STRONG_IN_OFF) //pin cfgd as o/p drive so Input buffer is off.
//         return GPIO_MODE_OUT;
//     else if (gpio_get_cypdl_drive(pin) == CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW)
//         return GPIO_MODE_OPEN_DRAIN; 
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