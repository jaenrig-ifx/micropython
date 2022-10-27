
#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mphal.h"


// MTB includes
#include "cybsp.h"
#include "cyhal.h"


#include "modmachine.h"



typedef struct _machine_pin_obj_t {
    mp_obj_base_t base;
    uint8_t id;
 } machine_pin_obj_t;


// TODO: list all pins
STATIC const machine_pin_obj_t machine_pin_obj[2] = {
    {{&machine_pin_type}, P13_7}, // used as ouput
    {{&machine_pin_type}, P6_0}  // used as input
};


void machine_pin_init(void) {
    mp_printf(&mp_plat_print, "machine pin init\n");
}


void machine_pin_deinit(void) {
    mp_printf(&mp_plat_print, "machine pin deinit\n");
}


STATIC void machine_pin_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    mp_printf(print, "machine pin print\n");
    // mp_printf(&mp_plat_print, "machine pin print\n");
}

// static const mp_arg_t allowed_args[] = {
//     {MP_QSTR_mode,  MP_ARG_OBJ,                  {.u_rom_obj = MP_ROM_NONE}},
//     {MP_QSTR_pull,  MP_ARG_OBJ,                  {.u_rom_obj = MP_ROM_NONE}},
//     {MP_QSTR_value, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE}},
//     {MP_QSTR_alt,   MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0}},
// };

// constructor(id, ...)
mp_obj_t mp_pin_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {


    (void) cyhal_gpio_init(machine_pin_obj[1].id, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false); // just to test input pin read value ! How to read current value of outputs ?


    mp_printf(&mp_plat_print, "machine make new\n");

    const machine_pin_obj_t *self = &machine_pin_obj[0]; // TODO: find pin passed as parameter

    (void) cyhal_gpio_init(self->id, CYHAL_GPIO_DIR_OUTPUT, 
                           CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_ON);

    printf("n_args %u    n_kw : %u   ", n_args, n_kw);

    // mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    // mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // if (args[ARG_pull].u_obj != mp_const_none) {
    //     int pull = mp_obj_get_int(args[ARG_pull].u_obj);
    //     if (pull) {
    //         mp_raise_ValueError("Pulls are not supported for this pin");
    //     }
    // }

    // if (args[ARG_alt].u_int != GPIO_FUNC_SIO) {
    //     mp_raise_ValueError("Alternate functions are not supported for this pin");
    // }

    // int value = -1;
    // if (args[ARG_value].u_obj != mp_const_none) {
    //     value = mp_obj_is_true(args[ARG_value].u_obj);
    // }

    // if (args[ARG_mode].u_obj != mp_const_none) {

    for(uint8_t i = 0; i < n_args; ++i) {
        if (mp_obj_is_str(args[i])) {
            const char *name = mp_obj_str_get_str(args[i]);
            printf("arg %u : %s   ", i, name);
        }
        else if  (mp_obj_is_int(args[i])) {
            uint32_t param = mp_obj_get_int(args[i]);
            printf("arg %u : %ld   ", i, param);

            if ( param == CYHAL_GPIO_DIR_OUTPUT ) {
                printf("\noutput detected\n");
            }
            else if( param == CYHAL_GPIO_DIR_INPUT ) {
                printf("\ninput detected\n");
            }
        }
    }

    return MP_OBJ_FROM_PTR(self);
};


//STATIC mp_obj_t machine_pin_call(mp_obj_t self_in, size_t n_args, size_t n_kw, const mp_obj_t *args) {
STATIC mp_obj_t machine_pin_call(mp_obj_t self_in, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *args) {
    mp_printf(&mp_plat_print, "machine pin call\n");
    // inputs are pulled to 1 if unconnected
    const machine_pin_obj_t *self = &machine_pin_obj[1]; // TODO: extract self from list of available pins

    return( MP_OBJ_NEW_SMALL_INT(cyhal_gpio_read(self->id)) );
 }


// pin.value([value])
STATIC mp_obj_t machine_pin_value(size_t n_args, const mp_obj_t *args) {
    return machine_pin_call(args[0], n_args - 1, 0, args + 1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_pin_value_obj, 1, 2, machine_pin_value);


STATIC mp_obj_t machine_pin_obj_init(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    mp_printf(&mp_plat_print, "machine pin obj init\n");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(machine_pin_init_obj, 1, machine_pin_obj_init);


// pin.toggle()
STATIC mp_obj_t machine_pin_toggle(mp_obj_t self_in) {
//    machine_pin_obj_t *self = MP_OBJ_TO_PTR(self_in);
    const machine_pin_obj_t *self = &machine_pin_obj[0]; // TODO: extract self from list of available pins
 
    cyhal_gpio_toggle(self->id);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_pin_toggle_obj, machine_pin_toggle);


STATIC const mp_rom_map_elem_t machine_pin_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init),    MP_ROM_PTR(&machine_pin_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_toggle),  MP_ROM_PTR(&machine_pin_toggle_obj) },
    { MP_ROM_QSTR(MP_QSTR_value),   MP_ROM_PTR(&machine_pin_value_obj) },


    // class constants
    { MP_ROM_QSTR(MP_QSTR_IN), MP_ROM_INT(CYHAL_GPIO_DIR_INPUT) },
    { MP_ROM_QSTR(MP_QSTR_OUT), MP_ROM_INT(CYHAL_GPIO_DIR_OUTPUT) },
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
