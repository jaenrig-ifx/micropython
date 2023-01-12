
// mpy includes
#include "extmod/machine_i2c.h"
#include "py/runtime.h"


// port-specific includes
#include "modules/machine/modmachine.h"
#include "psoc6_i2c.h"



void i2c_init(mp_obj_t self_in) {
// void i2c_init(int scl, int sda, uint32_t freq)
    machine_i2c_obj_t *self = MP_OBJ_TO_PTR(self_in);

    // printf("i2c init : scl : %d   sda : %d   freq : %lu\n", scl, sda, freq);
    printf("i2c init : scl : %d   sda : %d   freq : %lu\n", self->scl, self->sda, self->freq);

    // Define the I2C master configuration structure
    cyhal_i2c_cfg_t i2c_master_config =
    {
        CYHAL_I2C_MODE_MASTER,
        0, // address is not used for master mode
        self->freq
    };

    // self->i2c_obj = (cyhal_i2c_t *) malloc(sizeof(cyhal_i2c_t));
    self->i2c_obj = mp_obj_malloc(cyhal_i2c_t, &machine_i2c_type);

    // Initialize I2C master, set the SDA and SCL pins and assign a new clock
    cy_rslt_t result = cyhal_i2c_init(self->i2c_obj, self->sda, self->scl, NULL);
    //  cy_rslt_t rslt = cyhal_i2c_init(&i2c_master_obj, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL);

    if (result != CY_RSLT_SUCCESS) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("cyhal_i2c_init failed with return code %d !"), result);
        // mp_raise_ValueError(MP_ERROR_TEXT("cyhal_i2c_init failed !"));
    }

    // Configure the I2C resource to be master
    result = cyhal_i2c_configure(self->i2c_obj, &i2c_master_config);
    // rslt = cyhal_i2c_configure(&i2c_master_obj, &i2c_master_config);

    if (result != CY_RSLT_SUCCESS) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("cyhal_i2c_configure failed with return code %d !"), result);
        // mp_raise_ValueError(MP_ERROR_TEXT("cyhal_i2c_configure failed !"));
    }
}
