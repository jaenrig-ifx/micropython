// std includes
#include <stdio.h>


// micropython includes
#include "genhdr/mpversion.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/mpconfig.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "py/stackctrl.h"
#include "shared/readline/readline.h"
#include "shared/runtime/pyexec.h"


// MTB includes
#include "cybsp.h"
#include "cyhal.h"
#include "cy_retarget_io.h"

// port-specific includes
#include "mplogger.h"

// port-specific includes
#include "mplogger.h"


cyhal_rtc_t psoc6_rtc;


#if MICROPY_ENABLE_GC

//static char *stack_top;
// TODO: set to proper value for our MCU
static char heap[192 * 1024];

#endif


int main(int argc, char **argv) {
    #if MICROPY_PY_THREAD
    mp_thread_init();
    #endif


    // We should capture stack top ASAP after start, and it should be
    // captured guaranteedly before any other stack variables are allocated.
    // For this, actual main (renamed main_) should not be inlined into
    // this function. main_() itself may have other functions inlined (with
    // their own stack variables), that's why we need this main/main_ split.
    mp_stack_ctrl_init();


   #ifdef SIGPIPE
    // Do not raise SIGPIPE, instead return EPIPE. Otherwise, e.g. writing
    // to peer-closed socket will lead to sudden termination of MicroPython
    // process. SIGPIPE is particularly nasty, because unix shell doesn't
    // print anything for it, so the above looks like completely sudden and
    // silent termination for unknown reason. Ignoring SIGPIPE is also what
    // CPython does. Note that this may lead to problems using MicroPython
    // scripts as pipe filters, but again, that's what CPython does. So,
    // scripts which want to follow unix shell pipe semantics (where SIGPIPE
    // means "pipe was requested to terminate, it's not an error"), should
    // catch EPIPE themselves.
    signal(SIGPIPE, SIG_IGN);
    #endif


//    // Define a reasonable stack limit to detect stack overflow.
//     mp_uint_t stack_limit = 40000 * (sizeof(void *) / 4);
//     #if defined(__arm__) && !defined(__thumb2__)
//     // ARM (non-Thumb) architectures require more stack.
//     stack_limit *= 2;
//     #endif
//     mp_stack_set_limit(stack_limit);


    cy_rslt_t result;

#if defined(CY_DEVICE_SECURE)

    cyhal_wdt_t wdt_obj;

    /* Clear watchdog timer so that it doesn't trigger a reset */
    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    cyhal_wdt_free(&wdt_obj);

    #endif /* #if defined (CY_DEVICE_SECURE) */


    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS) {
        CY_ASSERT(0);
    }


    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* retarget-io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS) {
        CY_ASSERT(0);
    }

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    mp_hal_stdout_tx_str(MICROPY_BANNER_NAME_AND_VERSION);
    mp_hal_stdout_tx_str("; " MICROPY_BANNER_MACHINE);
    mp_hal_stdout_tx_str("\nUse Ctrl-D to exit, Ctrl-E for paste mode\n");

    setvbuf( stdin,  NULL, _IONBF, 0 );
    setvbuf( stdout, NULL, _IONBF, 0 );
    
    // int stack_dummy;

soft_reset:

    // stack_top = (char *)&stack_dummy;

    #if MICROPY_ENABLE_GC

    gc_init(heap, heap + sizeof(heap));

    #endif

    mp_init();

    // mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_lib));
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_flash));
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_flash_slash_lib));

    readline_init0();

    // indicate in REPL console when debug mode is selected
    mplogger_print("\n...LOGGER DEBUG MODE...\n\n");

//    pyexec_frozen_module("vfs_lfs2.py");
    // pyexec_frozen_module("vfs_fat.py");

    #if MICROPY_VFS_FAT
    // pyexec_frozen_module("vfs_fat.py");
    // #else
    pyexec_frozen_module("vfs_lfs2.py");
    #endif

    // Execute user scripts.
    int ret = pyexec_file_if_exists("flash/boot.py");

    if (ret & PYEXEC_FORCED_EXIT) {
        goto soft_reset;
    }

    if (pyexec_mode_kind == PYEXEC_MODE_FRIENDLY_REPL) {
        ret = pyexec_file_if_exists("flash/main.py");

        if (ret & PYEXEC_FORCED_EXIT) {
            goto soft_reset;
        }
    }

    __enable_irq();

    for (;;) {
       if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
            if (pyexec_raw_repl() != 0) {
                break;
            }
        } else {
            if (pyexec_friendly_repl() != 0) {
                break;
            }
        }
    }

    printf("MPY: soft reboot\n");

    mp_deinit();

    goto soft_reset;

    return 0;
}


// TODO: to be implemented
void nlr_jump_fail(void *val) {
    printf("nlr_jump_fail\n");

    while (1) {
        ;
    }
}
