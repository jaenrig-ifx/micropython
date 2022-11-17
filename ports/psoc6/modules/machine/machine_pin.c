#include "py/runtime.h"
#include "py/mphal.h"

#include "pins.h"

#include "mphalport.h"
#include "modmachine.h"


enum { ARG_dir, ARG_drive, ARG_value };
    
static const mp_arg_t allowed_args[] = {
    {MP_QSTR_dir,   MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE}},
    {MP_QSTR_drive, MP_ARG_OBJ, {.u_obj = MP_ROM_NONE}},
    {MP_QSTR_value, MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE}}
    
    //!TODO: Alternate functions of pin available? Check proto datasheet.
    //{MP_QSTR_alt,   MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = GPIO_FUNC_SIO}},
};

// Pin objects
const machine_pin_obj_t machine_pin_obj[] = {
    {{&machine_pin_type}, PIN_P13_7, "P13_7"},
};

// Mandatory MPY functions
STATIC mp_obj_t machine_pin_call(mp_obj_t self_in, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *args) {
    mp_printf(&mp_plat_print, "machine pin call\n");
    return mp_const_none;
}

STATIC void machine_pin_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    mp_printf(&mp_plat_print, "machine pin print\n");
}

mp_obj_t machine_pin_obj_init_helper(const machine_pin_obj_t *self, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    // Parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    
    mp_hal_pin_rslt_t result = mp_hal_pin_init(self->id,mp_obj_get_int(args[ARG_dir].u_obj),mp_obj_get_int(args[ARG_drive].u_obj),mp_obj_get_int(args[ARG_value].u_obj));

    //!TODO: Add logger support to print returns/msgs/errors.
    if(result != CY_RSLT_SUCCESS)
    {
        mp_printf(&mp_plat_print, "Init Status returned = %d\n", result);
        mp_printf(&mp_plat_print, "Init unsuccessful\n");
    }
    return mp_const_none;
}


// Machine Pin methods - port-specific definitions
// Pin Contructor
mp_obj_t mp_pin_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    // Inspects the arguement list and checks number of arguements - Function signature from mpy side
    // Signature: mp_arg_check_num(n_args, n_kw, self->n_args_min, self->n_args_max, self->is_kw);
    mp_arg_check_num(n_args, n_kw, 4, 4, true);
    
    // get the wanted LED object
    int wanted_led = pin_find(args[0], (const machine_pin_obj_t *)machine_pin_obj, MP_ARRAY_SIZE(machine_pin_obj));
    const machine_pin_obj_t *self = NULL;
    if (0 <= wanted_led && wanted_led < MP_ARRAY_SIZE(machine_pin_obj)) {
        self = (machine_pin_obj_t *)&machine_pin_obj[wanted_led];
    }

    if (self == NULL || self->base.type == NULL) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid Pin"));
    }
   
    // Create a map directly from the given args array
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, args + n_args); 
    machine_pin_obj_init_helper(self, n_args - 1, args + 1, &kw_args);

    return MP_OBJ_FROM_PTR(self);
}

// Pin init
STATIC mp_obj_t machine_pin_init(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    return machine_pin_obj_init_helper(args[0], n_args - 1, args + 1, kw_args);
}
MP_DEFINE_CONST_FUN_OBJ_KW(machine_pin_init_obj, 1, machine_pin_init);

// Toggle on board led
STATIC mp_obj_t machine_pin_toggle_onBoardLed() {
    mp_hal_pin_toggle_onBoardLed();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_pin_toggle_onBoardLed_obj, 0,machine_pin_toggle_onBoardLed);

//!TODO: Do we need to pass pin here? pin.toggle() no? Hw can we access this directly internally? Global var for self->id maybe assigned during constructor creation?
STATIC mp_obj_t machine_pin_toggle(mp_obj_t pin) {
    machine_pin_obj_t *self = MP_OBJ_TO_PTR(pin);
    mp_hal_pin_toggle(self->id);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_pin_toggle_obj, machine_pin_toggle);


STATIC const mp_rom_map_elem_t machine_pin_locals_dict_table[] = {
    // Instance methods
    { MP_ROM_QSTR(MP_QSTR___name__),               MP_ROM_QSTR(MP_QSTR_umachine) },
    { MP_ROM_QSTR(MP_QSTR_init),                   MP_ROM_PTR(&machine_pin_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_toggle_onBoardLed),      MP_ROM_PTR(&machine_pin_toggle_onBoardLed_obj) },
    { MP_ROM_QSTR(MP_QSTR_toggle),                 MP_ROM_PTR(&machine_pin_toggle_obj) },

    // Pin-direction constants
    { MP_ROM_QSTR(MP_QSTR_IN),                     MP_ROM_INT(CYHAL_GPIO_DIR_INPUT) },        
    { MP_ROM_QSTR(MP_QSTR_OUT),                    MP_ROM_INT(CYHAL_GPIO_DIR_OUTPUT) },       
    { MP_ROM_QSTR(MP_QSTR_BI_DIR),                 MP_ROM_INT(CYHAL_GPIO_DIR_BIDIRECTIONAL) },

    // Pin drive mode constants
    //!TODO: Complete the list when implementing GPIO
    { MP_ROM_QSTR(MP_QSTR_DRIVE_STRONG),           MP_ROM_INT(CYHAL_GPIO_DRIVE_STRONG) },  

    //Pin initial value constants
    { MP_ROM_QSTR(MP_QSTR_STATE_HIGH),             MP_ROM_INT(GPIO_STATE_ON) },  
    { MP_ROM_QSTR(MP_QSTR_STATE_LOW),              MP_ROM_INT(GPIO_STATE_OFF) },   

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