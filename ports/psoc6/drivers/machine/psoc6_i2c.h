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
    cyhal_i2c_t i2c_obj;
    uint8_t i2c_id;
    uint8_t scl;
    uint8_t sda;
    uint32_t freq;
} machine_i2c_obj_t;

cy_rslt_t i2c_init(cyhal_i2c_t *i2c_obj, cyhal_gpio_t scl, cyhal_gpio_t sda, uint32_t freq);
cy_rslt_t i2c_read(cyhal_i2c_t i2c_obj, uint16_t addr, size_t len, uint8_t *buf, bool stop);
cy_rslt_t i2c_write(cyhal_i2c_t i2c_obj, uint16_t addr, size_t len, uint8_t *buf, bool stop);

#endif // MICROPY_INCLUDED_PSOC6_I2C_H
