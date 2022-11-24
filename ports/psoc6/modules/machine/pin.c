
#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mphal.h"


// MTB includes
#include "cybsp.h"
#include "cyhal.h"


#include "modmachine.h"


/* 
  test functionality  
  

import machine
from machine import Pin
a=Pin("P13_7", Pin.IN, "Pin.PULL_UP")

# toggle red LED
a.toggle()
a.toggle()

a.init()

# should give 1
a.value()

print(a)

# then connect P6_0 to GNG and
# should yield 0
a.value()
  
*/
// #if MICROPY_DEBUG_VERBOSE // print debugging info
// #define DEBUG_PRINT (1)
// #else // don't print debugging info
// #define DEBUG_PRINT (0)
// #define DEBUG_printf(...) (void)0
// #endif

// STATIC const uint16_t hash_allocation_sizes[] = {
//     0, 2, 4, 6, 8, 10, 12, // +2
//     17, 23, 29, 37, 47, 59, 73, // *1.25
//     97, 127, 167, 223, 293, 389, 521, 691, 919, 1223, 1627, 2161, // *1.33
//     3229, 4831, 7243, 10861, 16273, 24407, 36607, 54907, // *1.5
// };

// STATIC size_t get_hash_alloc_greater_or_equal_to(size_t x) {
//     for (size_t i = 0; i < MP_ARRAY_SIZE(hash_allocation_sizes); i++) {
//         if (hash_allocation_sizes[i] >= x) {
//             return hash_allocation_sizes[i];
//         }
//     }
//     // ran out of primes in the table!
//     // return something sensible, at least make it odd
//     return (x + x / 2) | 1;
// }


// STATIC void mp_map_rehash(mp_map_t *map) {
//     size_t old_alloc = map->alloc;
//     size_t new_alloc = get_hash_alloc_greater_or_equal_to(map->alloc + 1);
//     DEBUG_printf("mp_map_rehash(%p): " UINT_FMT " -> " UINT_FMT "\n", map, old_alloc, new_alloc);
//     mp_map_elem_t *old_table = map->table;
//     mp_map_elem_t *new_table = m_new0(mp_map_elem_t, new_alloc);
//     // If we reach this point, table resizing succeeded, now we can edit the old map.
//     map->alloc = new_alloc;
//     map->used = 0;
//     map->all_keys_are_qstrs = 1;
//     map->table = new_table;
//     for (size_t i = 0; i < old_alloc; i++) {
//         if (old_table[i].key != MP_OBJ_NULL && old_table[i].key != MP_OBJ_SENTINEL) {
//             mp_map_lookup(map, old_table[i].key, MP_MAP_LOOKUP_ADD_IF_NOT_FOUND)->value = old_table[i].value;
//         }
//     }
//     m_del(mp_map_elem_t, old_table, old_alloc);
// }



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
//     { MP_QSTR_mode, MP_ARG_OBJ, {.u_int = 10} },
//     { MP_QSTR_pull, MP_ARG_OBJ, {.u_int = 11} },
//     // {MP_QSTR_mode,  MP_ARG_OBJ,                  {.u_rom_obj = MP_ROM_NONE}},
//     // {MP_QSTR_pull,  MP_ARG_OBJ,                  {.u_rom_obj = MP_ROM_NONE}},
//     // {MP_QSTR_value, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE}},
//     {MP_QSTR_alt,   MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0}},
// };

enum {
    ARG_mode, ARG_pull, ARG_value, ARG_alt
};
static const mp_arg_t allowed_args[] = {
    {MP_QSTR_mode,  MP_ARG_OBJ,                  {.u_rom_obj = MP_ROM_NONE}},
    {MP_QSTR_pull,  MP_ARG_OBJ,                  {.u_rom_obj = MP_ROM_NONE}},
    {MP_QSTR_value, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE}},
    {MP_QSTR_alt,   MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 123}},
};

STATIC mp_obj_t machine_pin_obj_init_helper(const machine_pin_obj_t *self, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // set initial value (do this before configuring mode/pull)
    if (args[ARG_value].u_obj != mp_const_none) {
//        gpio_put(self->id, mp_obj_is_true(args[ARG_value].u_obj));
    }

    // configure mode
    if (args[ARG_mode].u_obj != mp_const_none) {
        mp_int_t mode = mp_obj_get_int(args[ARG_mode].u_obj);
        printf("mode : %d\n", mode);
//         if (mode == GPIO_MODE_IN) {
//             mp_hal_pin_input(self->id);
//         } else if (mode == GPIO_MODE_OUT) {
//             mp_hal_pin_output(self->id);
//         } else if (mode == GPIO_MODE_OPEN_DRAIN) {
//             mp_hal_pin_open_drain(self->id);
//         } else {
//             // Alternate function.
//  //           gpio_set_function(self->id, args[ARG_alt].u_int);
//             machine_pin_open_drain_mask &= ~(1 << self->id);
//         }
    }

    // configure pull (unconditionally because None means no-pull)
    uint32_t pull = 0;
    if (args[ARG_pull].u_obj != mp_const_none) {
        pull = mp_obj_get_int(args[ARG_pull].u_obj);
        printf("pull : %ld\n", pull);
    }
//    gpio_set_pulls(self->id, pull & GPIO_PULL_UP, pull & GPIO_PULL_DOWN);
    return mp_const_none;
}

// constructor(id, ...)
mp_obj_t mp_pin_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_printf(&mp_plat_print, "machine make new\n");

    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    (void) cyhal_gpio_init(machine_pin_obj[1].id, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false); // just to test input pin read value ! How to read current value of outputs ?



    const machine_pin_obj_t *self = &machine_pin_obj[0]; // TODO: find pin passed as parameter

    (void) cyhal_gpio_init(self->id, CYHAL_GPIO_DIR_OUTPUT, 
                           CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_ON);

    printf("n_args : %u    n_kw : %u\n", n_args, n_kw);


    for(uint8_t i = 0; i < n_args + n_kw + 1; ++i) {
        if (mp_obj_is_str(args[i])) {
            const char *name = mp_obj_str_get_str(args[i]);
            printf("arg str %u : %s\n", i, name);
        }
        else if  (mp_obj_is_int(args[i])) {
            uint32_t param = mp_obj_get_int(args[i]);
            printf("arg int %u : %ld\n", i, param);

            if ( param == CYHAL_GPIO_DIR_OUTPUT ) {
                printf("\noutput detected\n");
            }
            else if( param == CYHAL_GPIO_DIR_INPUT ) {
                printf("\ninput detected\n");
            }
        }
    }

    printf("\nn_args : %u    n_kw : %u\n", n_args, n_kw);

    if (n_args >= 1 || n_kw > 0) {
        // pin mode given, so configure this GPIO
        mp_map_t kw_args;
        mp_map_init_fixed_table(&kw_args, n_kw, args + n_args);
        machine_pin_obj_init_helper(self, n_args - 1, args + 1, &kw_args);
    }
    // mp_map_t kw_args;
    // mp_map_init_fixed_table(&kw_args, n_kw, args + n_args);

    // machine_pin_obj_init_helper(self, n_args - 1, args + 1, &kw_args);


    // for(uint8_t i = 0; i < n_kw; ++i) {
    //     printf("kw arg %d : %d", i, kw_args[i].u_int);
    // } 
    printf("n_args : %u    n_kw : %u\n", n_args, n_kw);

    // mp_arg_val_t args_array[MP_ARRAY_SIZE(allowed_args)];
    // mp_arg_parse_all(n_args, args, &kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args_array);

    // for(uint8_t i = 0; i < n_kw; ++i) {
    //     printf("kw arg %d : %d", i, args_array[i].u_int);
    // } 


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
    mp_printf(&mp_plat_print, "machine pin value\n");

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
    mp_printf(&mp_plat_print, "machine pin toggle\n");

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
    { MP_ROM_QSTR(MP_QSTR_IN),     MP_ROM_INT(CYHAL_GPIO_DIR_INPUT) },
    { MP_ROM_QSTR(MP_QSTR_OUT),    MP_ROM_INT(CYHAL_GPIO_DIR_OUTPUT) },
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
