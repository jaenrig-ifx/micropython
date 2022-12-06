// std includes
#include <stdio.h>
#include <stdlib.h>

// port-specific includes
#include "modmachine.h"


/* Include Pin definitions to make it visible at py side*/
#define PIN_P13_7 CYHAL_GET_GPIO(CYHAL_PORT_13, 7)
#define PIN_P0_4  CYHAL_GET_GPIO(CYHAL_PORT_0, 4)

// Add all machine pin objects - GPIO , I2C, ADC etc.
typedef struct _machine_pin_obj_t {
    mp_obj_base_t base;
    uint32_t pin_addr;
    char *pin_name;
} machine_pin_obj_t;

// Function Prototypes to support interaction between c<->py
int pin_find(mp_obj_t obj, const machine_pin_obj_t *pin_obj, int num);
