
#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mphal.h"


// MTB includes
#include "cybsp.h"
#include "cyhal.h"

#include "modmachine.h"
#include "mphalport.h"


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

typedef struct _machine_pin_obj_t {
    mp_obj_base_t base;
    //char * id;
    uint8_t id;
} machine_pin_obj_t;

STATIC const machine_pin_obj_t machine_pin_obj[] = {
    //{{&machine_pin_type}, P0_0},
    {{&machine_pin_type}, 1},
    {{&machine_pin_type}, 2},
    {{&machine_pin_type}, 13}
};

//STATIC const machine_pin_obj_t *led_pin_obj = &machine_pin_obj[3];

void machine_pin_init(void) {
    mp_printf(&mp_plat_print, "machine pin init\n");
}


void machine_pin_deinit(void) {
    mp_printf(&mp_plat_print, "machine pin deinit\n");
}


STATIC void machine_pin_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    mp_printf(&mp_plat_print, "machine pin print\n");
}

mp_obj_t machine_pin_obj_init_helper(const machine_pin_obj_t *self, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    enum { ARG_mode, ARG_pull, ARG_value, ARG_drive };
    
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_mode,  MP_ARG_OBJ,                  {.u_rom_obj = MP_ROM_NONE}},
        {MP_QSTR_pull,  MP_ARG_OBJ,                  {.u_rom_obj = MP_ROM_NONE}},
        {MP_QSTR_value, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE}},
        {MP_QSTR_drive, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL}}
        //TODO: Alternate functions of pin available? Check proto datasheet.
        //{MP_QSTR_alt,   MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = GPIO_FUNC_SIO}},
    };

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    return mp_const_none;
}

mp_obj_t mp_pin_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    // get the wanted pin object
    int wanted_pin = mp_obj_str_get_str(args[0]);
    const machine_pin_obj_t *self = NULL;
    if (0 <= wanted_pin && wanted_pin < MP_ARRAY_SIZE(machine_pin_obj)) {
        self = (machine_pin_obj_t *)&machine_pin_obj[wanted_pin];
    }
    if (self == NULL || self->base.type == NULL) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid pin"));
    }

    if (n_args > 1 || n_kw > 0) {
        // pin mode given, so configure this GPIO
        mp_map_t kw_args;
        mp_map_init_fixed_table(&kw_args, n_kw, args + n_args);
        machine_pin_obj_init_helper(self, n_args - 1, args + 1, &kw_args);
    }

    return MP_OBJ_FROM_PTR(self);
}


STATIC mp_obj_t machine_pin_call(mp_obj_t self_in, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *args) {
    mp_printf(&mp_plat_print, "machine pin call\n");
    return mp_const_none;
}

STATIC mp_obj_t machine_pin_obj_init(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    mp_printf(&mp_plat_print, "machine pin obj init\n");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(machine_pin_init_obj, 1, machine_pin_obj_init);

// pin.toggle()
//STATIC mp_obj_t machine_pin_toggle(mp_obj_t self_in) {
STATIC mp_obj_t machine_pin_toggle(mp_obj_t self_in) {
    machine_pin_obj_t *self = MP_OBJ_TO_PTR(self_in);
    //machine_pin_obj_t *self = self_in;
    mp_hal_pin_toggle_led(self->id);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_pin_toggle_obj, machine_pin_toggle);

STATIC const mp_rom_map_elem_t machine_pin_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR___name__),     MP_ROM_QSTR(MP_QSTR_umachine) },
    { MP_ROM_QSTR(MP_QSTR_init),         MP_ROM_PTR(&machine_pin_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_toggle),       MP_ROM_PTR(&machine_pin_toggle_obj) },
    
    // Pin-event constants
    { MP_ROM_QSTR(MP_QSTR_IRQ_NONE), MP_ROM_INT(CYHAL_GPIO_IRQ_NONE) },
    { MP_ROM_QSTR(MP_QSTR_IRQ_RISE), MP_ROM_INT(CYHAL_GPIO_IRQ_RISE) },
    { MP_ROM_QSTR(MP_QSTR_IRQ_FALL), MP_ROM_INT(CYHAL_GPIO_IRQ_FALL) },
    { MP_ROM_QSTR(MP_QSTR_IRQ_BOTH), MP_ROM_INT(CYHAL_GPIO_IRQ_BOTH) },

    // Pin-direction constants
    { MP_ROM_QSTR(MP_QSTR_IN), MP_ROM_INT(CYHAL_GPIO_DIR_INPUT) },        
    { MP_ROM_QSTR(MP_QSTR_OUT), MP_ROM_INT(CYHAL_GPIO_DIR_OUTPUT) },       
    { MP_ROM_QSTR(MP_QSTR_BI_DIR), MP_ROM_INT(CYHAL_GPIO_DIR_BIDIRECTIONAL) },
    
     //Pin-drive mode constants
    { MP_ROM_QSTR(MP_QSTR_DRV_NONE), MP_ROM_INT(CYHAL_GPIO_DRIVE_NONE) },               
    { MP_ROM_QSTR(MP_QSTR_DRV_AN), MP_ROM_INT(CYHAL_GPIO_DRIVE_ANALOG) },             
    { MP_ROM_QSTR(MP_QSTR_DRV_PULLUP), MP_ROM_INT(CYHAL_GPIO_DRIVE_PULLUP) },            
    { MP_ROM_QSTR(MP_QSTR_DRV_PULLDOWN), MP_ROM_INT(CYHAL_GPIO_DRIVE_PULLDOWN) },           
    { MP_ROM_QSTR(MP_QSTR_DRV_OPENDRAINDRIVESLOW), MP_ROM_INT(CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW) },
    { MP_ROM_QSTR(MP_QSTR_DRV_OPENDRAINDRIVESHIGH), MP_ROM_INT(CYHAL_GPIO_DRIVE_OPENDRAINDRIVESHIGH) },
    { MP_ROM_QSTR(MP_QSTR_DRV_STRONG), MP_ROM_INT(CYHAL_GPIO_DRIVE_STRONG) },             
    { MP_ROM_QSTR(MP_QSTR_DRV_PULLUPDOWN), MP_ROM_INT(CYHAL_GPIO_DRIVE_PULLUPDOWN) },         
    { MP_ROM_QSTR(MP_QSTR_DRV_PULLNONE), MP_ROM_INT(CYHAL_GPIO_DRIVE_PULL_NONE) }  
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
