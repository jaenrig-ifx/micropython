
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


// MTB includes
#include "cybsp.h"
#include "cyhal.h"


#include "modmachine.h"


// #include "uart.h"
// #include "hardware/clocks.h"
// #include "hardware/pll.h"
// #include "hardware/structs/rosc.h"
// #include "hardware/structs/scb.h"
// #include "hardware/structs/syscfg.h"
// #include "hardware/watchdog.h"
// #include "hardware/xosc.h"
// #include "pico/bootrom.h"
// #include "pico/stdlib.h"
// #include "pico/unique_id.h"



#if MICROPY_PY_MACHINE



STATIC mp_obj_t machine_reset(void) {
    mp_printf(&mp_plat_print, "machine reset\n");
    printf("machine reset\n");

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_reset_obj, machine_reset);


STATIC mp_obj_t machine_soft_reset(void) {
    mp_printf(&mp_plat_print, "machine soft_reset\n");
    printf("machine soft_reset\n");

    pyexec_system_exit = PYEXEC_FORCED_EXIT;
//    mp_raise_type(&mp_type_SystemExit);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_soft_reset_obj, machine_soft_reset);

STATIC mp_obj_t machine_reset_cause(void) {
    mp_printf(&mp_plat_print, "machine reset_cause\n");
    printf("machine reset_cause\n");

    int reset_cause = 5;
    return MP_OBJ_NEW_SMALL_INT(reset_cause);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_reset_cause_obj, machine_reset_cause);


NORETURN mp_obj_t machine_bootloader(size_t n_args, const mp_obj_t *args) {
    mp_printf(&mp_plat_print, "machine bootloader\n");
    printf("machine bootloader\n");

    MICROPY_BOARD_ENTER_BOOTLOADER(n_args, args);

    for (;;) {
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_bootloader_obj, 0, 1, machine_bootloader);


STATIC mp_obj_t machine_lightsleep(void) {
    mp_printf(&mp_plat_print, "machine machine_lightsleep\n");
    printf("machine machine_lightsleep\n");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_lightsleep_obj, machine_lightsleep);

STATIC mp_obj_t machine_deepsleep(void) {
    mp_printf(&mp_plat_print, "machine machine_deepsleep\n");
    printf("machine machine_deepsleep\n");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_deepsleep_obj, machine_deepsleep);

STATIC mp_obj_t machine_enable_irq(void) {
    mp_printf(&mp_plat_print, "machine machine_enable_irq\n");
    printf("machine machine_enable_irq\n");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_enable_irq_obj, machine_enable_irq);

// Resets the board in a manner similar to pushing the external RESET button.
STATIC mp_obj_t machine_disable_irq(void) {
    mp_printf(&mp_plat_print, "machine machine_disable_irq\n");
    printf("machine machine_disable_irq\n");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_disable_irq_obj, machine_disable_irq);


void machine_init(void) {
    mp_printf(&mp_plat_print, "machine init\n");
}


void machine_deinit(void) {
    mp_printf(&mp_plat_print, "machine deinit\n");
}

extern unsigned int __bss_start__, __bss_end__, __data_start__, __data_end__, __HeapLimit, __StackLimit, __StackTop, __etext;
// uint32_t 


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
        printf("qstr :\n  n_pool=%u\n  n_qstr=%u\n  n_str_data_bytes=%u\n  n_total_bytes=%u\n", n_pool, n_qstr, n_str_data_bytes, n_total_bytes);
    }

    // GC info
    {
        gc_info_t info;
        gc_info(&info);
        printf("GC :\n");
        printf("  %u total\n", info.total);
        printf("  %u : %u\n", info.used, info.free);
        printf("  1=%u 2=%u m=%u\n", info.num_1block, info.num_2block, info.max_block);
    }

    // Linker info
    {
       printf("Linker :\n");
       printf("  %x   %u  __etext\n", __etext, __etext);

       printf("  %x   %u  __bss_start__\n", __bss_start__, __bss_start__);
       printf("  %x   %u  __bss_end__\n", __bss_end__, __bss_end__);

       printf("  %x   %u  __data_start__\n", __data_start__, __data_start__);
       printf("  %x   %u  __data_end__\n", __data_end__, __data_end__);
       printf("  %x   %u  data size\n", __data_end__ - __data_start__, __data_end__ - __data_start__);

       printf("  %x   %u  __HeapLimit\n", __HeapLimit, __HeapLimit);
       printf("  %x   %u  __StackLimit\n", __StackLimit, __StackLimit);
       printf("  %x   %u  __StackTop\n", __StackTop, __StackTop);
   }

    return mp_const_none;
}
//STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_info_obj, machine_info);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_info_obj, 0, 1, machine_info);



STATIC const mp_rom_map_elem_t machine_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),            MP_ROM_QSTR(MP_QSTR_umachine) },

    { MP_ROM_QSTR(MP_QSTR_reset),               MP_ROM_PTR(&machine_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_soft_reset),          MP_ROM_PTR(&machine_soft_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_reset_cause),         MP_ROM_PTR(&machine_reset_cause_obj) },
    { MP_ROM_QSTR(MP_QSTR_bootloader),          MP_ROM_PTR(&machine_bootloader_obj) },
    { MP_ROM_QSTR(MP_QSTR_enable_irq),         MP_ROM_PTR(&machine_enable_irq_obj) },
    { MP_ROM_QSTR(MP_QSTR_disable_irq),        MP_ROM_PTR(&machine_disable_irq_obj) },
    { MP_ROM_QSTR(MP_QSTR_idle),               MP_ROM_PTR(&machine_lightsleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep),              MP_ROM_PTR(&machine_lightsleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_lightsleep),         MP_ROM_PTR(&machine_lightsleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_deepsleep),          MP_ROM_PTR(&machine_deepsleep_obj) },

    { MP_ROM_QSTR(MP_QSTR_info),                MP_ROM_PTR(&machine_info_obj) },

    { MP_ROM_QSTR(MP_QSTR_Pin),                 MP_ROM_PTR(&machine_pin_type) }
};
STATIC MP_DEFINE_CONST_DICT(machine_module_globals, machine_module_globals_table);


const mp_obj_module_t mp_module_machine = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&machine_module_globals,
};


MP_REGISTER_MODULE(MP_QSTR_umachine, mp_module_machine);


#endif // MICROPY_PY_MACHINE
