
#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mphal.h"


// MTB includes
#include "cybsp.h"
#include "cyhal.h"


#include "modmachine.h"


// #define GPIO_MODE_IN (0)
// #define GPIO_MODE_OUT (1)
// #define GPIO_MODE_OPEN_DRAIN (2)
// #define GPIO_MODE_ALT (3)

// // These can be or'd together.
// #define GPIO_PULL_UP (1)
// #define GPIO_PULL_DOWN (2)

// #define GPIO_IRQ_ALL (0xf)

// // Macros to access the state of the hardware.
// #define GPIO_GET_FUNCSEL(id) ((iobank0_hw->io[(id)].ctrl & IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS) >> IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB)
// #define GPIO_IS_OUT(id) (sio_hw->gpio_oe & (1 << (id)))
// #define GPIO_IS_PULL_UP(id) (padsbank0_hw->io[(id)] & PADS_BANK0_GPIO0_PUE_BITS)
// #define GPIO_IS_PULL_DOWN(id) (padsbank0_hw->io[(id)] & PADS_BANK0_GPIO0_PDE_BITS)

// // Open drain behaviour is simulated.
// #define GPIO_IS_OPEN_DRAIN(id) (machine_pin_open_drain_mask & (1 << (id)))


void machine_pin_init(void) {
    mp_printf(&mp_plat_print, "machine pin init\n");
}


void machine_pin_deinit(void) {
    mp_printf(&mp_plat_print, "machine pin deinit\n");
}


STATIC void machine_pin_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    mp_printf(&mp_plat_print, "machine pin print\n");
}


// constructor(id, ...)
mp_obj_t mp_pin_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_printf(&mp_plat_print, "machine make new\n");
    (void) cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, 
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    cyhal_gpio_toggle(CYBSP_USER_LED);
    return mp_const_none;
};


STATIC mp_obj_t machine_pin_call(mp_obj_t self_in, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *args) {
    mp_printf(&mp_plat_print, "machine pin call\n");
    return mp_const_none;
 }


STATIC mp_obj_t machine_pin_obj_init(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    mp_printf(&mp_plat_print, "machine pin obj init\n");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(machine_pin_init_obj, 1, machine_pin_obj_init);


STATIC const mp_rom_map_elem_t machine_pin_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init),    MP_ROM_PTR(&machine_pin_init_obj) },
};

STATIC MP_DEFINE_CONST_DICT(machine_pin_locals_dict, machine_pin_locals_dict_table);


MP_DEFINE_CONST_OBJ_TYPE(
    machine_pin_type,
    MP_QSTR_Pin,
    MP_TYPE_FLAG_NONE,
    make_new, mp_pin_make_new,
    print, machine_pin_print,
    call, machine_pin_call,
    locals_dict, &machine_pin_locals_dict
    );
