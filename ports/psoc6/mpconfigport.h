/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020-2021 Damien P. George
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

// Options controlling how MicroPython is built, overriding defaults in py/mpconfig.h
#include <stdint.h>
#include "mpconfigboard.h"

// Board and hardware specific configuration
#define MICROPY_HW_MCU_NAME                     "PSoC62"
#define MICROPY_HELPER_REPL                     (1)
#define MICROPY_HW_ENABLE_UART_REPL             (0) // useful if there is no USB
#define MICROPY_HW_ENABLE_USBDEV                (0)

#ifndef MICROPY_CONFIG_ROM_LEVEL
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_FULL_FEATURES)
#endif

// Memory allocation policies
#define MICROPY_GC_STACK_ENTRY_TYPE             uint16_t
#define MICROPY_ALLOC_PATH_MAX                  (256)
#define MICROPY_QSTR_BYTES_IN_HASH              (1)

// MicroPython emitters
#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_EMIT_THUMB                      (1)
#define MICROPY_EMIT_THUMB_ARMV7M               (1)
#define MICROPY_EMIT_INLINE_THUMB               (1)
#define MICROPY_EMIT_INLINE_THUMB_FLOAT         (1)
#define MICROPY_EMIT_ARM                        (0)

// Optimisations
#define MICROPY_OPT_COMPUTED_GOTO               (0)

// Python internal features
#define MICROPY_READER_VFS                      (1)
#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF  (1)

#define MICROPY_LONGINT_IMPL                    (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
// #define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_DOUBLE)

#define MICROPY_SCHEDULER_DEPTH                 (8)
#define MICROPY_SCHEDULER_STATIC_NODES          (1)

#ifndef MICROPY_USE_INTERNAL_ERRNO
#define MICROPY_USE_INTERNAL_ERRNO              (0)
#endif

// Fine control over Python builtins, classes, modules, etc
#define MICROPY_PY_BUILTINS_HELP_TEXT           psoc6_help_text
#define MICROPY_PY_SYS_PLATFORM                 "psoc6"
#define MICROPY_PY_THREAD                       (0)
#define MICROPY_PY_THREAD_GIL                   (0)

// Extended modules
#define MICROPY_EPOCH_IS_1970                   (1)
#define MICROPY_PY_UOS_INCLUDEFILE              "ports/psoc6/modules/os/moduos.c"
#define MICROPY_PY_UOS_UNAME                    (1)
#define MICROPY_PY_UOS_URANDOM                  (0)
#define MICROPY_PY_UOS_GETENV_PUTENV_UNSETENV   (1)
#define MICROPY_PY_UOS_SYSTEM                   (1)
#define MICROPY_PY_UOS_ERRNO                    (1)

#define MICROPY_PY_URE_MATCH_GROUPS             (1)
#define MICROPY_PY_URE_MATCH_SPAN_START_END     (1)
#define MICROPY_PY_UTIME_MP_HAL                 (1)
#define MICROPY_PY_MACHINE                      (1)
#define MICROPY_PY_MACHINE_PIN_MAKE_NEW         mp_pin_make_new
#define MICROPY_PY_MACHINE_BITSTREAM            (0)
#define MICROPY_PY_MACHINE_PULSE                (0)
#define MICROPY_PY_MACHINE_PWM                  (0)
#define MICROPY_PY_MACHINE_PWM_DUTY_U16_NS      (0)
#define MICROPY_PY_MACHINE_I2C                  (1)
#define MICROPY_PY_MACHINE_SOFTI2C              (1)
#define MICROPY_PY_MACHINE_I2C_TRANSFER_WRITE1  (0)

#define MICROPY_PY_MACHINE_SPI                  (0)
#define MICROPY_PY_MACHINE_SOFTSPI              (0)
#define MICROPY_PY_ONEWIRE                      (0)
#define MICROPY_VFS                             (1)

#define MICROPY_VFS_FAT                         (0)

// fatfs configuration
#define MICROPY_FATFS_ENABLE_LFN                (1)
#define MICROPY_FATFS_LFN_CODE_PAGE             437 /* 1=SFN/ANSI 437=LFN/U.S.(OEM) */
#define MICROPY_FATFS_RPATH                     (2)

// set to 1 to enable filesystem to be loaded on external qspi flash
// if set to 0, filesystem is located in an allotted area of internal flash of PSoC6
#define MICROPY_ENABLE_EXT_QSPI_FLASH               (1)

#ifndef MICROPY_BOARD_ENTER_BOOTLOADER
#define MICROPY_BOARD_ENTER_BOOTLOADER(nargs, args)
#endif

#define MICROPY_TRACKED_ALLOC        (MICROPY_SSL_MBEDTLS)

#define MICROPY_PY_UCRYPTOLIB        (1)
#define MICROPY_PY_UCRYPTOLIB_CTR    (1)
#define MICROPY_PY_UCRYPTOLIB_CONSTS (1)

#define MICROPY_PY_UHASHLIB          (1)
#define MICROPY_PY_UHASHLIB_MD5      (1)
#define MICROPY_PY_UHASHLIB_SHA1     (1)
#define MICROPY_PY_UHASHLIB_SHA256   (1)

#define MP_STATE_PORT MP_STATE_VM

// By default networking should include sockets, ssl, websockets, webrepl
#if MICROPY_PY_NETWORK

#ifndef MICROPY_PY_USOCKET
#define MICROPY_PY_USOCKET              (1)
#endif

#ifndef MICROPY_PY_USSL
#define MICROPY_PY_USSL                 (1)
#endif

#ifndef MICROPY_PY_UWEBSOCKET
#define MICROPY_PY_UWEBSOCKET           (1)
#endif

#ifndef MICROPY_PY_UHASHLIB_SHA1
#define MICROPY_PY_UHASHLIB_SHA1        (0)
#endif

#ifndef MICROPY_PY_WEBREPL
#define MICROPY_PY_WEBREPL              (1)
#endif

#ifndef MICROPY_PY_WEBREPL
#define MICROPY_PY_WEBREPL              (1)
#endif

extern const struct _mp_obj_type_t mp_network_ifx_whd_type;
#define MICROPY_HW_NIC_IFX_WHD  \
    { MP_ROM_QSTR(MP_QSTR_WLAN), MP_ROM_PTR(&mp_network_ifx_whd_type) },

#define MICROPY_PORT_NETWORK_INTERFACES \
    MICROPY_HW_NIC_IFX_WHD
#endif

// Miscellaneous settings
#define MICROPY_MAKE_POINTER_CALLABLE(p) ((void *)((mp_uint_t)(p) | 1))

#define MP_SSIZE_MAX (0x7fffffff)
typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef intptr_t mp_off_t;

// We need to provide a declaration/definition of alloca()
#include <alloca.h>


////////////////////////////////////////////////////////////////////////
// TODO: Remove options implicitly set by setting feature level. Must work for all feature levels !
// the following entries have been added by JB
// Usually passed from Makefile
#ifndef MICROPY_GC_HEAP_SIZE
#define MICROPY_GC_HEAP_SIZE (256 * 1024)
#endif


#define MICROPY_LOGGER_DEBUG              (0)


#define MICROPY_PY_BUILTINS_HELP          (1)
#define MICROPY_PY_BUILTINS_HELP_MODULES  (1)



#define MICROPY_ENABLE_COMPILER           (1)
#define MICROPY_QSTR_EXTRA_POOL           mp_qstr_frozen_const_pool
#define MICROPY_ALLOC_PARSE_CHUNK_INIT    (16)
#define MICROPY_KBD_EXCEPTION             (1)

#define MICROPY_ENABLE_SOURCE_LINE        (1)

#define MICROPY_USE_INTERNAL_PRINTF       (0)
#define MICROPY_ENABLE_FINALISER          (1)
#define MICROPY_VFS_POSIX                 (0)
#define MICROPY_READER_POSIX              (0)


#define MICROPY_REPL_INFO                 (1)

// Set to zero explicitly ! Or mounting the /flash fs must be done inside try/except for both branches !
// #define MICROPY_MODULE_BUILTIN_INIT    (0)

#define MICROPY_PY_MICROPYTHON            (1)
#define MICROPY_PY_GC                     (1)
#define MICROPY_PY_IO                     (1)
#define MICROPY_PY_IO_IOBASE              (1)


#define MICROPY_PY_USELECT                (1)
#define MICROPY_PY_USELECT_SELECT         (1)

#define MICROPY_STACK_CHECK               (1)

// TODO: helpful to abstract main.c ?
// #define MICROPY_PORT_INIT_FUNC ??
// #define MICROPY_PORT_DEINIT_FUNC ??


#include "shared/runtime/interrupt_char.h"


#define MICROPY_OBJ_REPR (MICROPY_OBJ_REPR_A)

#define MICROPY_EVENT_POLL_HOOK_FAST \
    do { \
        extern void mp_handle_pending(bool); \
        mp_handle_pending(true); \
    } while (0)

#define MICROPY_EVENT_POLL_HOOK \
    do { \
        MICROPY_EVENT_POLL_HOOK_FAST; \
    } while (0);

// best_effort_wfe_or_timeout(make_timeout_time_ms(1));



#define MICROPY_PY_SYS_EXC_INFO      (1)

#define MICROPY_PY_URE_DEBUG                (1)
#define MICROPY_PY_URE_MATCH_GROUPS         (1)
#define MICROPY_PY_URE_MATCH_SPAN_START_END (1)

#define MICROPY_MEM_STATS (1)
#define MICROPY_MALLOC_USES_ALLOCATED_SIZE (1)
