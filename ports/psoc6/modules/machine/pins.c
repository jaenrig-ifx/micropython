
#include "modmachine.h"
#include "pins.h"

// Function definitions
int pin_find(mp_obj_t pin, const machine_pin_obj_t machine_pin_obj[], int table_size) {
    int wanted_pin = -1;
    if (mp_obj_is_small_int(pin)) {
        // Pin defined by the index of pin table
        wanted_pin = mp_obj_get_int(pin);
    } else if (mp_obj_is_str(pin)) {
        // Search by name
        size_t slen;
        const char *s = mp_obj_str_get_data(pin, &slen);
        for (wanted_pin = 0; wanted_pin < table_size; wanted_pin++) {
            if (slen == strlen(machine_pin_obj[wanted_pin].name) &&
                strncmp(s, machine_pin_obj[wanted_pin].name, slen) == 0) {
                break;
            }
        }
    }
    return wanted_pin;
}