
// mpy includes
#include "extmod/machine_i2c.h"
#include "py/runtime.h"


// MTB includes


// port-specific includes
#include "drivers/machine/psoc6_gpio.h"
#include "modules/machine/modmachine.h"
#include "mplogger.h"
#include "psoc6_i2c.h"

extern void mp_hal_i2c_delay(machine_i2c_obj_t *self);

cy_rslt_t i2c_init(cyhal_i2c_t *i2c_obj, cyhal_gpio_t scl, cyhal_gpio_t sda, uint32_t freq) {

    // Define the I2C master configuration structure
    cyhal_i2c_cfg_t i2c_master_config =
    {
        CYHAL_I2C_MODE_MASTER,
        0, // address is not used for master mode
        freq
    };

    // Initialize I2C master, set the SDA and SCL pins and assign a new clock
    cy_rslt_t result = cyhal_i2c_init(i2c_obj, sda, scl, NULL);

    return result == CY_RSLT_SUCCESS ? cyhal_i2c_configure(i2c_obj, &i2c_master_config)
                                     : result;
}


cy_rslt_t i2c_read(cyhal_i2c_t *i2c_obj, uint16_t addr, uint8_t *buf, size_t len, uint32_t timeout, bool send_stop) {
    return cyhal_i2c_master_read(i2c_obj, addr, buf, len, timeout, send_stop);
}


cy_rslt_t i2c_write(cyhal_i2c_t *i2c_obj, uint16_t addr, uint8_t *buf, size_t len, uint32_t timeout, bool send_stop) {
    return cyhal_i2c_master_write(i2c_obj, addr, buf, len, timeout, send_stop);
}
