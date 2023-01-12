#ifndef MICROPY_INCLUDED_PSOC6_I2C_H
#define MICROPY_INCLUDED_PSOC6_I2C_H

// std includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


// MTB includes
#include "cybsp.h"
#include "cyhal.h"
#include "cy_pdl.h"




typedef struct _machine_i2c_obj_t {
    mp_obj_base_t base;
    cyhal_i2c_t *i2c_obj;
    uint8_t i2c_id;
    uint8_t scl;
    uint8_t sda;
    uint32_t freq;
} machine_i2c_obj_t;


// void i2c_init(int scl, int sda, uint32_t freq);
void i2c_init(mp_obj_t self_in);


#endif // MICROPY_INCLUDED_PSOC6_I2C_H
