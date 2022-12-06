/* CYHAL SYSTEM functions */

//std includes
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "psoc6_system.h"

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

// API to return clock freq; Fast CLK (CM4) is the main sys clk
uint32_t system_get_cpu_freq(void) {
//    return Cy_SysClk_ClkPathMuxGetFrequency(Cy_SysClk_ClkPathGetSource(0UL));
    return Cy_SysClk_ClkFastGetFrequency();
}

// API to return clock freq divider for Fast CLK (CM4)
uint8_t system_get_cpu_freq_div(void) {
    return Cy_SysClk_ClkFastGetDivider();
}