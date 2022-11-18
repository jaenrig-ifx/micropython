
// std includes
#include <stdint.h>
#include <stdio.h>
#include <string.h>


// micropython includes
#include "py/builtin.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/repl.h"
#include "py/runtime.h"
#include "shared/readline/readline.h"
#include "shared/runtime/pyexec.h"


// MTB includes
#include "cybsp.h"
#include "cyhal.h"
#include "cy_retarget_io.h"


#if MICROPY_ENABLE_COMPILER

void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;

    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, true);

        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}

#endif


#if MICROPY_ENABLE_GC

static char *stack_top;
// TODO: set to proper value for our MCU
static char heap[192 * 1024];

#endif


int main(int argc, char **argv) {
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

    // /* Enable global interrupts */
    // __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* retarget-io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS) {
        CY_ASSERT(0);
    }


    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    // printf("\x1b[2J\x1b[;H");

    // printf("****************** "
    //        "HAL: Hello World! Example "
    //        "****************** \r\n\n");
    // printf("Hello World!!!\r\n\n");

    setvbuf(stdin,  NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    int stack_dummy;

soft_reset:

    stack_top = (char *)&stack_dummy;

    #if MICROPY_ENABLE_GC

    gc_init(heap, heap + sizeof(heap));

    #endif

    mp_init();
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_lib));
    readline_init0();

    #if MICROPY_ENABLE_COMPILER

    // #if MICROPY_REPL_EVENT_DRIVEN

    // pyexec_event_repl_init();

    for (;;) {
        // int c = mp_hal_stdin_rx_chr();

        // if (pyexec_event_repl_process_char(c)) {
        //     break;
        // }

        // if( ret != 0 ) {
        //     // printf("EOF!!!   %i   %i\n", c, ret);
        //     pyexec_event_repl_init();
        // }
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

    // #else

    // printf("\r\nrunning pyexec_friendly_repl ...\r\n");
    // pyexec_friendly_repl();

    // #endif

    #else

    #endif

    printf("MPY: soft reboot\n");

    mp_deinit();

    goto soft_reset;

    return 0;
}


#if MICROPY_ENABLE_GC

void gc_collect(void) {
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
}

#endif


// TODO: to be implemented
mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    mp_raise_OSError(MP_ENOENT);
}


// TODO: to be implemented
mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}


// TODO: to be implemented
void nlr_jump_fail(void *val) {
    while (1) {
        ;
    }
}


// TODO: to be implemented
mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);
