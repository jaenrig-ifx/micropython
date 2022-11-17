/* Include Pin definitions to make it visible at py side*/
#define PIN_P13_7 CYHAL_GET_GPIO(CYHAL_PORT_13, 7)

// Add all machine pin objects - GPIO , I2C, ADC etc.
typedef struct _machine_pin_obj_t {
    mp_obj_base_t base;
    uint32_t id;
    char *name;
} machine_pin_obj_t;

// Function Prototypes to support interaction between c<->py
int pin_find(mp_obj_t pin, const machine_pin_obj_t machine_pin_obj[], int table_size);