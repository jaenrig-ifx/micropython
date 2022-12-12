#ifndef MICROPY_INCLUDED_PSOC6_MODMACHINE_H
#define MICROPY_INCLUDED_PSOC6_MODMACHINE_H

// std includes
#include <stdio.h>

// mpy includes
#include "py/obj.h"
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

// port-specific includes
#include "drivers/psoc6_system.h"

extern const mp_obj_type_t machine_pin_type;

/* Note: the static functions' prototypes in the .c file cannot be declared here
since they are static. The static type in those functions come from MPY hence
should stay that way.  */

// functions to be called from other .c files. Hence they are not static. 
void machine_init(void); 
void machine_deinit(void); 

#endif // MICROPY_INCLUDED_PSOC6_MODMACHINE_H
