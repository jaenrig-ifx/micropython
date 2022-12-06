#ifndef MICROPY_INCLUDED_PSOC6_MODMACHINE_H
#define MICROPY_INCLUDED_PSOC6_MODMACHINE_H

// Standard includes
#include <stdio.h>

// MPY interpreter related includes
#include "py/gc.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "shared/runtime/pyexec.h"
#include "py/obj.h"

//port-specific includes
#include "drivers/psoc6_system.h"

extern const mp_obj_type_t machine_pin_type;

#endif // MICROPY_INCLUDED_PSOC6_MODMACHINE_H
