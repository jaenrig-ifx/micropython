/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 "Krzysztof Adamski" <k@japko.eu>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

#include <time.h>
#include <sys/time.h>


#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "py/mperrno.h"
#include "shared/timeutils/timeutils.h"
#include "py/misc.h"


#include "cyhal.h"


#include "modmachine.h"


// object defined in main
extern cyhal_rtc_t psoc6_rtc;


typedef struct _machine_rtc_obj_t {
    mp_obj_base_t base;
} machine_rtc_obj_t;


// singleton RTC object
STATIC const machine_rtc_obj_t machine_rtc_obj = {{&machine_rtc_type}};


STATIC mp_obj_t machine_rtc_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 0, 0, false);
    bool r = cyhal_rtc_is_enabled(&psoc6_rtc);

    if (!r) {
        // This shouldn't happen as rtc_init() is already called in main so
        // it's here just in case
        cy_rslt_t result = cyhal_rtc_init(&psoc6_rtc);

        if (CY_RSLT_SUCCESS != result) {
//            mp_raise_NotImplementedError(mp_rom_error_text_t "cyhal_rtc_init failed !");
            printf("cyhal_rtc_init failed !");
        }

        struct tm current_date_time = { .tm_mon = 1, .tm_mday = 1 };
        result = cyhal_rtc_write(&psoc6_rtc, &current_date_time);

        if (CY_RSLT_SUCCESS != result) {
            // mp_raise_NotImplementedError("cyhal_rtc_write failed !");
            printf("cyhal_rtc_write failed !");
        }
    }

    // return constant object
    return (mp_obj_t)&machine_rtc_obj;
}

STATIC mp_obj_t machine_rtc_datetime(mp_uint_t n_args, const mp_obj_t *args) {
    if (n_args == 1) {
        struct tm current_date_time = {0};
        cy_rslt_t result = cyhal_rtc_read(&psoc6_rtc, &current_date_time);

        if (CY_RSLT_SUCCESS != result) {
            // mp_raise_NotImplementedError("cyhal_rtc_read failed !");
            printf("cyhal_rtc_read failed !");
        }

        mp_obj_t tuple[8] = {
            mp_obj_new_int(current_date_time.tm_year),
            mp_obj_new_int(current_date_time.tm_mon),
            mp_obj_new_int(current_date_time.tm_mday),
            mp_obj_new_int(current_date_time.tm_wday),
            mp_obj_new_int(current_date_time.tm_hour),
            mp_obj_new_int(current_date_time.tm_min),
            mp_obj_new_int(current_date_time.tm_sec),
            mp_obj_new_int(0)
        };

        return mp_obj_new_tuple(8, tuple);
    } else {
        mp_obj_t *items;

        mp_obj_get_array_fixed_n(args[1], 8, &items);

        struct tm current_date_time =
        {
            .tm_year = mp_obj_get_int(items[0]),
            .tm_mon = mp_obj_get_int(items[1]),
            .tm_mday = mp_obj_get_int(items[2]),
            .tm_hour = mp_obj_get_int(items[4]),
            .tm_min = mp_obj_get_int(items[5]),
            .tm_sec = mp_obj_get_int(items[6]),
        };

        // Deliberately ignore the weekday argument and compute the proper value
        current_date_time.tm_wday = timeutils_calc_weekday(current_date_time.tm_year, current_date_time.tm_mon, current_date_time.tm_mday);

        cy_rslt_t result = cyhal_rtc_write(&psoc6_rtc, &current_date_time);

        if (CY_RSLT_SUCCESS != result) {
            // mp_raise_NotImplementedError("cyhal_rtc_write failed !");
            printf("cyhal_rtc_write failed !");
        }

    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_rtc_datetime_obj, 1, 2, machine_rtc_datetime);


STATIC const mp_rom_map_elem_t machine_rtc_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_datetime), MP_ROM_PTR(&machine_rtc_datetime_obj) },
};
STATIC MP_DEFINE_CONST_DICT(machine_rtc_locals_dict, machine_rtc_locals_dict_table);


MP_DEFINE_CONST_OBJ_TYPE(
    machine_rtc_type,
    MP_QSTR_RTC,
    MP_TYPE_FLAG_NONE,
    make_new, machine_rtc_make_new,
    locals_dict, &machine_rtc_locals_dict
    );
