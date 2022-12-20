// std includes
#include <stdio.h>


// micropython includes
#include "genhdr/mpversion.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/mpconfig.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "py/stackctrl.h"
#include "shared/readline/readline.h"
#include "shared/runtime/pyexec.h"


// MTB includes
#include "cybsp.h"
#include "cyhal.h"
#include "cy_retarget_io.h"


// port-specific includes
#include "mplogger.h"


extern cyhal_rtc_t psoc6_rtc;


#if MICROPY_ENABLE_GC

extern uint8_t __StackTop, __StackLimit;

__attribute__((section(".bss"))) static char gc_heap[MICROPY_GC_HEAP_SIZE];

#endif


int main(int argc, char **argv) {
    #if MICROPY_PY_THREAD
    mp_thread_init();
    #endif


    #if MICROPY_ENABLE_GC

    mp_stack_set_top(&__StackTop);
    mp_stack_set_limit((mp_uint_t)&__StackTop - 256 - MICROPY_GC_HEAP_SIZE);
    gc_init(&gc_heap[0], &gc_heap[MP_ARRAY_SIZE(gc_heap)]);

    #endif


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


    cy_rslt_t result;


    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS) {
        mp_raise_ValueError(MP_ERROR_TEXT("cybsp_init failed !\n"));
    }


    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* retarget-io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS) {
        mp_raise_ValueError(MP_ERROR_TEXT("cy_retarget_io_init failed !\n"));
    }

soft_reset:

    mp_init();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    mp_printf(&mp_plat_print, "\x1b[2J\x1b[;H");
    mp_hal_stdout_tx_str(MICROPY_BANNER_NAME_AND_VERSION);
    mp_hal_stdout_tx_str("; " MICROPY_BANNER_MACHINE);
    mp_hal_stdout_tx_str("\nUse Ctrl-D to exit, Ctrl-E for paste mode\n");

    setvbuf(stdin,  NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_flash));
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_flash_slash_lib));

    readline_init0();
    // TODO: other init functions
    // machine_pin_init();

    // indicate in REPL console when debug mode is selected
    mplogger_print("\n...LOGGER DEBUG MODE...\n\n");

    #if MICROPY_VFS_FAT
    pyexec_frozen_module("vfs_fat.py");

    // #else
    // pyexec_frozen_module("vfs_lfs2.py");
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

    mp_printf(&mp_plat_print, "MPY: soft reboot\n");

    // TODO: other deinit functions
    // machine_pin_deinit();
    gc_sweep_all();
    mp_deinit();

    goto soft_reset;

    return 0;
}


// TODO: to be implemented
void nlr_jump_fail(void *val) {
    mplogger_print("nlr_jump_fail\n");
    mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(val));

    for (;;) {
        __BKPT(0);
    }
}


const char psoc6_help_text[] =
    "Welcome to MicroPython!\n"
    "\n"
    "For online help please visit https://micropython.org/help/.\n"
    "\n"
    "For access to the hardware use the 'machine' module.  PSoC6 specific commands\n"
    "are in the 'psoc6' module.\n"
    "\n"
    "Quick overview of some objects:\n"
    "  machine.Pin(pin) -- get a pin, eg machine.Pin(0)\n"
    "  machine.Pin(pin, m, [p]) -- get a pin and configure it for IO mode m, pull mode p\n"
    "    methods: init(..), value([v]), high(), low(), irq(handler)\n"
    "  machine.I2C(id) -- create an I2C object (id=0,1)\n"
    "    methods: readfrom(addr, buf, stop=True), writeto(addr, buf, stop=True)\n"
    "             readfrom_mem(addr, memaddr, arg), writeto_mem(addr, memaddr, arg)\n"
    "\n"
    "Pin IO modes are: Pin.IN, Pin.OUT, Pin.ALT\n"
    "Pin pull modes are: Pin.PULL_UP, Pin.PULL_DOWN\n"
    "\n"
    "Useful control commands:\n"
    "  CTRL-C -- interrupt a running program\n"
    "  CTRL-D -- on a blank line, do a soft reset of the board\n"
    "  CTRL-E -- on a blank line, enter paste mode\n"
    "\n"
    "For further help on a specific object, type help(obj)\n"
    "For a list of available modules, type help('modules')\n"
;
