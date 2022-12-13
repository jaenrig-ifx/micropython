#include <stdint.h>

// options to control how MicroPython is built

// Use the minimal starting configuration (disables all optional features).
//#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_CORE_FEATURES)
//#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_EXTRA_FEATURES)
//#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_EVERYTHING)

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
#define MICROPY_PY_SYS_PLATFORM             "psoc6"

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

#define MICROPY_LONGINT_IMPL                    (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)

// MicroPython emitters
#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_EMIT_THUMB                      (1)
#define MICROPY_EMIT_THUMB_ARMV7M               (0)
#define MICROPY_EMIT_INLINE_THUMB               (1)
#define MICROPY_EMIT_INLINE_THUMB_FLOAT         (0)



// // Optimisations
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
#define MICROPY_PY_BUILTINS_HELP_MODULES (1)
#define MICROPY_PY_SYS_MODULES          (1)

#define MICROPY_PY_UOS                        (1)
#define MICROPY_PY_UOS_INCLUDEFILE            "ports/psoc6/modules/os/moduos.c"
#define MICROPY_PY_UOS_UNAME                  (1)
#define MICROPY_PY_UOS_URANDOM                (0)
#define MICROPY_PY_UOS_GETENV_PUTENV_UNSETENV (0)
#define MICROPY_PY_UOS_SYSTEM                 (1)

#define MICROPY_PY_UTIME                (1)
#define MICROPY_PY_UTIME_MP_HAL         (1)

#define MICROPY_PY_MICROPYTHON_MEM_INFO (1)
#define MICROPY_PY_MACHINE              (1)
#define MICROPY_MODULE_WEAK_LINKS       (1)
#define MICROPY_PY_STRUCT               (1)


#ifndef MICROPY_USE_INTERNAL_ERRNO
#define MICROPY_USE_INTERNAL_ERRNO              (0)
#endif

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


#define MICROPY_COMP_CONST_FOLDING       (0)
#define MICROPY_COMP_CONST               (0)
#define MICROPY_COMP_DOUBLE_TUPLE_ASSIGN (0)
#define MICROPY_COMP_MODULE_CONST (0)
#define MICROPY_QSTR_BYTES_IN_HASH              (1)


#ifndef MICROPY_BOARD_ENTER_BOOTLOADER
#define MICROPY_BOARD_ENTER_BOOTLOADER(nargs, args)
#endif

#define MICROPY_LOGGER_DEBUG                    (1)


#define MICROPY_ENABLE_FINALISER    (1)
#define MICROPY_VFS                 (1)
#define MICROPY_VFS_LFS2            (1)
#define MICROPY_VFS_FAT             (1)
#define MICROPY_READER_VFS          (1)
#define MICROPY_VFS_POSIX           (0)
#define MICROPY_READER_POSIX        (0)


// fatfs configuration
#define MICROPY_FATFS_ENABLE_LFN                (1)
#define MICROPY_FATFS_LFN_CODE_PAGE             437 /* 1=SFN/ANSI 437=LFN/U.S.(OEM) */
#define MICROPY_FATFS_RPATH                     (2)

#if MICROPY_HW_USB_MSC
#define MICROPY_FATFS_USE_LABEL                 (1)
#define MICROPY_FATFS_MULTI_PARTITION           (1)
// Set FatFS block size to flash sector size to avoid caching
// the flash sector in memory to support smaller block sizes.
#define MICROPY_FATFS_MAX_SS                    (0x200)
// #define MICROPY_FATFS_MAX_SS                    (FLASH_SECTOR_SIZE)
// #define MICROPY_FATFS_MAX_SS           (4096)
#endif

// VFS stat functions should return time values relative to 1970/1/1
// #define MICROPY_EPOCH_IS_1970       (1)


#ifndef MICROPY_PY_SYS_PATH_DEFAULT
#define MICROPY_PY_SYS_PATH_DEFAULT ".frozen:~/.micropython/lib:/usr/lib/micropython"
#endif


#define MICROPY_PY_THREAD                       (0)


#define MP_SSIZE_MAX (0x7fffffff)

#define MICROPY_PY_SYS (1)
#define MICROPY_PY_SYS_STDIO_BUFFER (1)


extern mp_uint_t begin_atomic_section();
extern void end_atomic_section(mp_uint_t state);

#define MICROPY_BEGIN_ATOMIC_SECTION()     begin_atomic_section()
#define MICROPY_END_ATOMIC_SECTION(state)  end_atomic_section(state)

#define MICROPY_PERSISTENT_CODE_SAVE (1)
#define MICROPY_PY_SYS_SETTRACE (1)
#define MICROPY_PY_SYS_EXC_INFO (1)
#define MICROPY_KBD_EXCEPTION (1)
#include "shared/runtime/interrupt_char.h"

#define MICROPY_PY_UERRNO (1)

#define MICROPY_REPL_INFO (1)
