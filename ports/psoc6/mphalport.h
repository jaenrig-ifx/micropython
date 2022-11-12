static inline mp_uint_t mp_hal_ticks_ms(void) {
    return 0;
}
static inline void mp_hal_set_interrupt_char(char c) {
}

//cy includes
#include "cy_sysclk.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_pdl.h"

//some mpy includes for mp_obj types
#include "py/runtime.h"

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

// //GPIO array to map to CYHAL pins //moved to machine pin file due to error of declared but never used.
extern cyhal_gpio_t CY_GPIO_array[CY_PROTO_062_4343_GPIOs]; 
//= {
//     (P13_7), //GPIO 0 - LED
//     (P0_4),  //GPIO 1 - BTN
// };

//function to get HSIOM config of a pin
static inline en_hsiom_sel_t PIN_GET_HSIOM_FUNC(uint8_t index){
    return Cy_GPIO_GetHSIOM(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index]));
}

//function to get cypdl drive modes, correlated to cyhal drive modes in file: cyhal_gpio.c
static inline uint32_t GPIO_GET_CYPDL_DRIVE(uint8_t index){
    return Cy_GPIO_GetDrivemode(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index]));
}

//function to check if pin is in mode Pin.OPEN_DRAIN. 
//drive comparisons done with PDL drive modes since function is from PDL (not HAL)
static inline bool GPIO_IS_OPEN_DRAIN(uint8_t index){
    // if (GPIO_GET_CYPDL_DRIVE(index) == CY_GPIO_DM_HIGHZ && Cy_GPIO_Read(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index])) == 1) //first condition of open- given in machine_pin init helper; compare to pdl drive modes (not hal drive modes)
    //     return 1;
    // else if (GPIO_GET_CYPDL_DRIVE(index) == CY_GPIO_DM_OD_DRIVESHIGH && Cy_GPIO_Read(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index])) == 0) //second condition of open-drain
    //     return 1;

    //Note: definition of open_drain changed in constructor hence above code might not be correct

    if(GPIO_GET_CYPDL_DRIVE(index) == CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW)
        return 1;
    else
        return 0;
}

//function to check if pin is in mode Pin.OUT; TODO: can be also implemented by checking input buffer on/off 
static inline bool GPIO_IS_OUT(uint8_t index){
    if(GPIO_GET_CYPDL_DRIVE(index) == CY_GPIO_DM_STRONG_IN_OFF){ //pin cfgd as o/p drive so Input buffer is off.
        return 1;
    }
    else{
        return 0;
    }
}

//function to check if pin is in mode Pin.IN; TODO: can be also implemented by checking input buffer on/off 
static inline bool GPIO_IS_IN(uint8_t index){
    if(GPIO_GET_CYPDL_DRIVE(index) == CY_GPIO_DM_HIGHZ)
        return 1;
    else
        return 0;
}


//function to check if pin has pull Pin.PULL_UP
static inline bool GPIO_IS_PULL_UP(uint8_t index){
    if(GPIO_GET_CYPDL_DRIVE(index) == CY_GPIO_DM_PULLUP_IN_OFF || GPIO_GET_CYPDL_DRIVE(index) == CY_GPIO_DM_PULLUP)
        return 1;
    else
        return 0;
}

//function to check if pin has pull Pin.PULL_DOWN
static inline bool GPIO_IS_PULL_DOWN(uint8_t index){
    if(GPIO_GET_CYPDL_DRIVE(index) == CY_GPIO_DM_PULLDOWN_IN_OFF || GPIO_GET_CYPDL_DRIVE(index) == CY_GPIO_DM_PULLDOWN)
        return 1;
    else
        return 0;
}

//function to check Pin.value
static inline uint8_t GPIO_GET_VALUE(uint8_t index){
    if(GPIO_IS_OUT(index)) //if Pin.Mode is Pin.OUT, read output driver
        return Cy_GPIO_ReadOut(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index]));
    else //if Pin.Mode is Pin.IN, read pin. 
        return Cy_GPIO_Read(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index]));
}

//function to call Pin.value only for pins with mode Pin.IN; used for __call__ function
//uses mp_const_none type for None/undefined return
static inline mp_obj_t GPIO_GET_VALUE_CALL(uint8_t index){
    if(GPIO_IS_IN(index)) //if Pin.Mode is Pin.IN, return current pin input value
        return MP_OBJ_NEW_SMALL_INT(Cy_GPIO_Read(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index])));
    else if(GPIO_IS_OUT(index)) //if Pin.Mode is Pin.OUT, then return is undefined 
        return mp_const_none; //undefined
    else{ //Pin.Mode is Pin.OPEN_DRAIN
            if (Cy_GPIO_ReadOut(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index])) == 0) //if 0 is driven in open_drain, then undefined
                return mp_const_none;
            else
                return MP_OBJ_NEW_SMALL_INT(Cy_GPIO_Read(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index])));
        }
}

//function to set Pin.value to 1; sets the output buffer which drives the output driver
static inline void GPIO_SET_VALUE(uint8_t index){
    return Cy_GPIO_Set(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index]));
}

//function to set Pin.value to 0; clear the output buffer which drives the output driver
static inline void GPIO_CLR_VALUE(uint8_t index){
    return Cy_GPIO_Clr(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index]));
}

//function to toggle Pin.value; toggle the output buffer which drives the output driver
static inline void GPIO_TOGGLE_VALUE(uint8_t index){
    return Cy_GPIO_Inv(CYHAL_GET_PORTADDR(CY_GPIO_array[index]),CYHAL_GET_PIN(CY_GPIO_array[index]));
}

//function to return 64-bit silicon ID of given PSoC microcontroller
// A combined 64-bit unique ID. [63:57] - DIE_YEAR [56:56] - DIE_MINOR [55:48] - DIE_SORT [47:40] - DIE_Y [39:32] - DIE_X [31:24] - DIE_WAFER [23:16] - DIE_LOT[2] [15: 8] - DIE_LOT[1] [ 7: 0] - DIE_LOT[0]
static inline uint64_t CYPDL_GET_UNIQUE_ID(void){
    return Cy_SysLib_GetUniqueId(); 
}

//using watchdog timer to count to minimum value (1ms) to trigger reset
//thread-safe way as other methods might interfere with pending interrupts, threads etc.
static inline void CYHAL_RESET(void){
    cyhal_wdt_t wdt_obj;
    cyhal_wdt_init(&wdt_obj, 1); //min 1ms count time
    cyhal_wdt_start(&wdt_obj);
}