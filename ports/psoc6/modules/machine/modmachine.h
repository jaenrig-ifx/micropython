#ifndef MICROPY_INCLUDED_PSOC6_MODMACHINE_H
#define MICROPY_INCLUDED_PSOC6_MODMACHINE_H

#include "py/obj.h"


extern const mp_obj_type_t machine_pin_type;


void machine_pin_init(void);
void machine_pin_deinit(void);


// void machine_init(void);
// void machine_deinit(void);


// MP_DECLARE_CONST_FUN_OBJ_VAR_BETWEEN(machine_info_obj);


#endif // MICROPY_INCLUDED_PSOC6_MODMACHINE_H
