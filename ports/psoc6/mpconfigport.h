#include <stdint.h>

// options to control how MicroPython is built

// Use the minimal starting configuration (disables all optional features).
//#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_CORE_FEATURES)

// You can disable the built-in MicroPython compiler by setting the following
// config option to 0.  If you do this then you won't get a REPL prompt, but you
// will still be able to execute pre-compiled scripts, compiled with mpy-cross.
#define MICROPY_ENABLE_COMPILER           (1)

#define MICROPY_QSTR_EXTRA_POOL           mp_qstr_frozen_const_pool
#define MICROPY_ENABLE_GC                 (1)
#define MICROPY_HELPER_REPL               (1)
#define MICROPY_MODULE_FROZEN_MPY         (1)
#define MICROPY_ENABLE_EXTERNAL_IMPORT    (1)

#define MICROPY_ALLOC_PATH_MAX            (256)
#define MICROPY_ALLOC_PARSE_CHUNK_INIT    (16)

// type definitions for the specific machine

typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef long mp_off_t;

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

#define MICROPY_HW_BOARD_NAME "CY8CPROTO-062-4343W"
#define MICROPY_HW_MCU_NAME   "PSoC62"

#ifdef __linux__
#define MICROPY_MIN_USE_STDOUT (1)
#endif

#ifdef __thumb__
#define MICROPY_MIN_USE_CORTEX_CPU (1)
#define MICROPY_MIN_USE_STM32_MCU (0)
#endif

#define MP_STATE_PORT MP_STATE_VM


////////////////////////////////////////////////////////////////////////
// the following entries have been added by JB
// Usually passed from Makefile
#ifndef MICROPY_HEAP_SIZE
#define MICROPY_HEAP_SIZE (192 * 1024)
#endif

#define MICROPY_ENABLE_SOURCE_LINE      (1)

#define MICROPY_USE_INTERNAL_PRINTF (0)

//#define MICROPY_LONGINT_IMPL                    (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)

// MicroPython emitters
#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_EMIT_THUMB                      (1)
#define MICROPY_EMIT_THUMB_ARMV7M               (0)
#define MICROPY_EMIT_INLINE_THUMB               (1)
#define MICROPY_EMIT_INLINE_THUMB_FLOAT         (0)



// // Optimisations
// #define MICROPY_OPT_COMPUTED_GOTO               (1)
#define MICROPY_ENABLE_SOURCE_LINE      (1)
#define MICROPY_PY_ARRAY                (1)
#define MICROPY_PY_BUILTINS_ENUMERATE   (1)
#define MICROPY_PY_BUILTINS_FILTER      (1)
#define MICROPY_PY_BUILTINS_MIN_MAX     (1)
#define MICROPY_PY_BUILTINS_PROPERTY    (1)
#define MICROPY_PY_BUILTINS_RANGE_ATTRS (1)
#define MICROPY_PY_BUILTINS_REVERSED    (1)
#define MICROPY_PY_BUILTINS_SET         (1)
#define MICROPY_PY_BUILTINS_HELP        (1)
#define MICROPY_PY_SYS_MODULES          (1)
#define MICROPY_PY_UTIME                (1)
#define MICROPY_PY_UTIME_MP_HAL         (1)

#define MICROPY_PY_MICROPYTHON_MEM_INFO (1)
#define MICROPY_PY_MACHINE              (1)
#define MICROPY_MODULE_WEAK_LINKS       (1)
#define MICROPY_PY_STRUCT               (1)

// #ifndef MICROPY_BOARD_ENTER_BOOTLOADER
// #define MICROPY_BOARD_ENTER_BOOTLOADER(nargs, args)
// #endif
extern const struct _mp_obj_module_t mp_module_machine;
extern const struct _mp_obj_module_t mp_module_time;

#if MICROPY_PY_UTIME
#define MICROPY_PY_UTIME_DEF \
        { MP_ROM_QSTR(MP_QSTR_utime), MP_ROM_PTR(&mp_module_time) },
#else
#define MICROPY_PY_UTIME_DEF
#endif

#define MICROPY_PORT_BUILTIN_MODULES \
    { MP_ROM_QSTR(MP_QSTR_machine), MP_ROM_PTR(&mp_module_machine) }, \
    MICROPY_PY_UTIME_DEF

#define MICROPY_PORT_BUILTIN_MODULE_WEAK_LINKS \
    { MP_ROM_QSTR(MP_QSTR_time), MP_ROM_PTR(&mp_module_time) }


#define MP_STATE_PORT MP_STATE_VM
//#define MICROPY_MODULE_FROZEN_STR       (1)
#define MICROPY_COMP_CONST_FOLDING      (0)
#define MICROPY_COMP_CONST              (0)
#define MICROPY_COMP_DOUBLE_TUPLE_ASSIGN (0)

#define MICROPY_QSTR_BYTES_IN_HASH              (1)
