
#include <stdio.h>


#include "py/gc.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "shared/runtime/pyexec.h"
#include "extmod/machine_bitstream.h"
#include "extmod/machine_i2c.h"
#include "extmod/machine_mem.h"
#include "extmod/machine_pulse.h"
#include "extmod/machine_pwm.h"
#include "extmod/machine_signal.h"
#include "extmod/machine_spi.h"

#include "modmachine.h"


#if MICROPY_PY_MACHINE

void machine_init(void) {
    mp_printf(&mp_plat_print, "machine init\n");
}


void machine_deinit(void) {
    mp_printf(&mp_plat_print, "machine deinit\n");
}


// machine.info([dump_alloc_table])
// Print out lots of information about the board.
STATIC mp_obj_t machine_info(size_t n_args, const mp_obj_t *args) {
    (void) cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, 
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    cyhal_gpio_toggle(CYBSP_USER_LED);
    mp_printf(&mp_plat_print, "info about machine\n");

    // qstr info
    {
        size_t n_pool, n_qstr, n_str_data_bytes, n_total_bytes;
        qstr_pool_info(&n_pool, &n_qstr, &n_str_data_bytes, &n_total_bytes);
        printf("qstr:\n  n_pool=%u\n  n_qstr=%u\n  n_str_data_bytes=%u\n  n_total_bytes=%u\n", n_pool, n_qstr, n_str_data_bytes, n_total_bytes);
    }

    // GC info
    {
        gc_info_t info;
        gc_info(&info);
        printf("GC:\n");
        printf("  %u total\n", info.total);
        printf("  %u : %u\n", info.used, info.free);
        printf("  1=%u 2=%u m=%u\n", info.num_1block, info.num_2block, info.max_block);
    }

    return mp_const_none;
}
//STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_info_obj, machine_info);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_info_obj, 0, 1, machine_info);

//machine.freq()
STATIC mp_obj_t machine_freq(size_t n_args, const mp_obj_t *args) {
    if (n_args == 0) {
        mp_printf(&mp_plat_print, "System core freq (CM4): %d Hz\n", mp_hal_get_cpu_freq());
        return mp_const_none;
    } else {
        mp_printf(&mp_plat_print, "Not implemented!!!\n");
        return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_freq_obj, 0, 1, machine_freq);

//machine.unique_id()

STATIC mp_obj_t machine_unique_id(void) {
    uint64_t id = CYPDL_GET_UNIQUE_ID();
    byte *id_addr = (byte *)&id;
    mp_printf(&mp_plat_print,"ID_formatted:%02x%02x%02x%02x:%02x%02x%02x%02x\n", id_addr[0], id_addr[1], id_addr[2], id_addr[3], id_addr[4], id_addr[5], id_addr[6], id_addr[7]);
    mp_printf(&mp_plat_print,"RAW_ID_data:");
    return mp_obj_new_bytes(id_addr,8); 
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_unique_id_obj, machine_unique_id);


STATIC const mp_rom_map_elem_t machine_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),            MP_ROM_QSTR(MP_QSTR_machine) },
    { MP_ROM_QSTR(MP_QSTR_info),                MP_ROM_PTR(&machine_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_freq),                MP_ROM_PTR(&machine_freq_obj) },
    { MP_ROM_QSTR(MP_QSTR_unique_id),           MP_ROM_PTR(&machine_unique_id_obj) },

    //TODO: dynamic memory allocation functions/objects. Not yet implemented
    // { MP_ROM_QSTR(MP_QSTR_mem8),                MP_ROM_PTR(&machine_mem8_obj) },
    // { MP_ROM_QSTR(MP_QSTR_mem16),               MP_ROM_PTR(&machine_mem16_obj) },
    // { MP_ROM_QSTR(MP_QSTR_mem32),               MP_ROM_PTR(&machine_mem32_obj) },

    { MP_ROM_QSTR(MP_QSTR_Pin),                 MP_ROM_PTR(&machine_pin_type) }
};
STATIC MP_DEFINE_CONST_DICT(machine_module_globals, machine_module_globals_table);


const mp_obj_module_t mp_module_machine = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&machine_module_globals,
};


MP_REGISTER_MODULE(MP_QSTR_umachine, mp_module_machine);


#endif // MICROPY_PY_MACHINE
