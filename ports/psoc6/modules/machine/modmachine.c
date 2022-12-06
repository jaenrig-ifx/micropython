// MPY structure includes
#include "modmachine.h"

#if MICROPY_PY_MACHINE


enum {MACHINE_PWRON_RESET, MACHINE_HARD_RESET, MACHINE_WDT_RESET, MACHINE_DEEPSLEEP_RESET, MACHINE_SOFT_RESET};

void machine_init(void) {
    mp_printf(&mp_plat_print, "machine init\n");
}


void machine_deinit(void) {
    mp_printf(&mp_plat_print, "machine deinit\n");
}


// machine.info([dump_alloc_table])
// Print out lots of information about the board.
STATIC mp_obj_t machine_info(size_t n_args, const mp_obj_t *args) {
    (void)cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT,
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
// STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_info_obj, machine_info);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_info_obj, 0, 1, machine_info);

// machine.freq()
STATIC mp_obj_t machine_freq(size_t n_args, const mp_obj_t *args) {
    if (n_args == 0) {
        mp_printf(&mp_plat_print, "System core freq (CM4): %d Hz\n", system_get_cpu_freq());
        return mp_const_none;
    } else {
        mp_printf(&mp_plat_print, "Not implemented!!!\n");
        return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_freq_obj, 0, 1, machine_freq);

// TODO: to be enabled as a part of Machine module
// //machine.unique_id()
// STATIC mp_obj_t machine_unique_id(void) {
//     uint64_t id = CYPDL_GET_UNIQUE_ID();
//     byte *id_addr = (byte *)&id;
//     mp_printf(&mp_plat_print,"ID_formatted:%02x%02x%02x%02x:%02x%02x%02x%02x\n", id_addr[0], id_addr[1], id_addr[2], id_addr[3], id_addr[4], id_addr[5], id_addr[6], id_addr[7]);
//     mp_printf(&mp_plat_print,"RAW_ID_data:");
//     return mp_obj_new_bytes(id_addr,8);
// }
// MP_DEFINE_CONST_FUN_OBJ_0(machine_unique_id_obj, machine_unique_id);

// machine.reset()
// STATIC mp_obj_t machine_reset(void) {
//     CYHAL_RESET();
//     return mp_const_none;
// }
// MP_DEFINE_CONST_FUN_OBJ_0(machine_reset_obj, machine_reset);

// machine.soft_reset()
// TODO: soft-reset must be handled in main.c and REPL has to deinit and reinit
STATIC mp_obj_t machine_soft_reset(void) {
    pyexec_system_exit = PYEXEC_FORCED_EXIT;
    mp_raise_type(&mp_type_SystemExit);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_soft_reset_obj, machine_soft_reset);

// machine.reset_cause()
// Note: accurate returns only after one forced hard reset post hex upload.
// STATIC mp_obj_t machine_reset_cause(void) {
//         qstr mp_reset_qstr = MP_QSTR_None;
//         uint8_t reset_cause_const = -1;
//         uint32_t reset_cause = CYPDL_RESET_CAUSE();

//         if(reset_cause == 0UL){
//             mp_reset_qstr = MP_QSTR_HARD_RESET;
//             reset_cause_const = MACHINE_HARD_RESET;
//         }
//         else if(reset_cause == CY_SYSLIB_RESET_HWWDT || reset_cause == CY_SYSLIB_RESET_SWWDT0 || reset_cause == CY_SYSLIB_RESET_SWWDT1 || reset_cause == CY_SYSLIB_RESET_SWWDT2 || reset_cause == CY_SYSLIB_RESET_SWWDT3 ){
//             mp_reset_qstr = MP_QSTR_WDT_RESET;
//             reset_cause_const = MACHINE_WDT_RESET;
//         }
//         else if(reset_cause == CY_SYSLIB_RESET_DPSLP_FAULT){
//             mp_reset_qstr = MP_QSTR_DEEPSLEEP_RESET;
//             reset_cause_const = MACHINE_DEEPSLEEP_RESET;
//         }
//         else if(reset_cause == CY_SYSLIB_RESET_SOFT){
//             mp_reset_qstr = MP_QSTR_SOFT_RESET;
//             reset_cause_const = MACHINE_SOFT_RESET;
//         }
//         else{
//             mp_reset_qstr = MP_QSTR_PWRON_RESET;
//             reset_cause_const = MACHINE_PWRON_RESET;
//         }

//         mp_printf(&mp_plat_print, "Reset cause: %q; ",mp_reset_qstr);
//     return MP_OBJ_NEW_SMALL_INT(reset_cause_const);
// }
// STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_reset_cause_obj, machine_reset_cause);

// machine.disable_irq()
// STATIC mp_obj_t machine_disable_irq(void) {
//     uint32_t state = CY_DISABLE_GLOBAL_IRQ();
//     mp_printf(&mp_plat_print,"IRQ State: ");
//     return mp_obj_new_int(state);
// }
// MP_DEFINE_CONST_FUN_OBJ_0(machine_disable_irq_obj, machine_disable_irq);

// machine.enable_irq()
// STATIC mp_obj_t machine_enable_irq(mp_obj_t state_in) {
//     uint32_t state = mp_obj_get_int(state_in);
//     bool result = CY_ENABLE_GLOBAL_IRQ(state);
//     if (result)
//         mp_printf(&mp_plat_print,"Interrupts enabled\n");
//     else
//         mp_printf(&mp_plat_print,"Interrupts not enabled; key mismatch!\n");
//     return mp_const_none;
// }
// MP_DEFINE_CONST_FUN_OBJ_1(machine_enable_irq_obj, machine_enable_irq);

// machine.idle()
// This executies a wfi machine instruction which reduces power consumption
// of the MCU until an interrupt occurs, at which point execution continues.
// see: https://www.infineon.com/dgdl/Infineon-AN219528_PSoC_6_MCU_low-power_modes_and_power_reduction_techniques-ApplicationNotes-v06_00-EN.pdf?fileId=8ac78c8c7cdc391c017d0d31efdc659f  pg.7
STATIC mp_obj_t machine_idle(void) {
    __WFI(); // standard ARM instruction
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_idle_obj, machine_idle);


STATIC const mp_rom_map_elem_t machine_module_globals_table[] = {
    // instance functions
    { MP_ROM_QSTR(MP_QSTR___name__),            MP_ROM_QSTR(MP_QSTR_machine) },
    { MP_ROM_QSTR(MP_QSTR_info),                MP_ROM_PTR(&machine_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_freq),                MP_ROM_PTR(&machine_freq_obj) },
//    { MP_ROM_QSTR(MP_QSTR_unique_id),           MP_ROM_PTR(&machine_unique_id_obj) },
//    { MP_ROM_QSTR(MP_QSTR_reset),               MP_ROM_PTR(&machine_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_soft_reset),          MP_ROM_PTR(&machine_soft_reset_obj) },
//    { MP_ROM_QSTR(MP_QSTR_reset_cause),         MP_ROM_PTR(&machine_reset_cause_obj) },

//    { MP_ROM_QSTR(MP_QSTR_disable_irq),         MP_ROM_PTR(&machine_disable_irq_obj) },
//    { MP_ROM_QSTR(MP_QSTR_enable_irq),          MP_ROM_PTR(&machine_enable_irq_obj) },

    { MP_ROM_QSTR(MP_QSTR_idle),                MP_ROM_PTR(&machine_idle_obj) },

    // { MP_ROM_QSTR(MP_QSTR_bootloader),          MP_ROM_PTR(&machine_bootloader_obj) }, // Note: not implemented

    // class constants
    { MP_ROM_QSTR(MP_QSTR_PWRON_RESET),         MP_ROM_INT(MACHINE_PWRON_RESET) },
    { MP_ROM_QSTR(MP_QSTR_HARD_RESET),          MP_ROM_INT(MACHINE_HARD_RESET) },
    { MP_ROM_QSTR(MP_QSTR_WDT_RESET),           MP_ROM_INT(MACHINE_WDT_RESET) },
    { MP_ROM_QSTR(MP_QSTR_DEEPSLEEP_RESET),     MP_ROM_INT(MACHINE_DEEPSLEEP_RESET) },
    { MP_ROM_QSTR(MP_QSTR_SOFT_RESET),          MP_ROM_INT(MACHINE_SOFT_RESET) },

    // TODO: dynamic memory allocation functions/objects. Not yet implemented
    // { MP_ROM_QSTR(MP_QSTR_mem8),                MP_ROM_PTR(&machine_mem8_obj) },
    // { MP_ROM_QSTR(MP_QSTR_mem16),               MP_ROM_PTR(&machine_mem16_obj) },
    // { MP_ROM_QSTR(MP_QSTR_mem32),               MP_ROM_PTR(&machine_mem32_obj) },

    // Modules
    { MP_ROM_QSTR(MP_QSTR_Pin),                 MP_ROM_PTR(&machine_pin_type) }
};
STATIC MP_DEFINE_CONST_DICT(machine_module_globals, machine_module_globals_table);


const mp_obj_module_t mp_module_machine = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&machine_module_globals,
};

// Register machine module in MPY
MP_REGISTER_MODULE(MP_QSTR_umachine, mp_module_machine);

#endif // MICROPY_PY_MACHINE
