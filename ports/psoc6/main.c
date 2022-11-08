
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



extern int mp_hal_stdin_rx_chr(void);
extern void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len);


#if MICROPY_ENABLE_COMPILER

void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;

    if (nlr_push(&nlr) == 0) {
        mp_lexer_t     *lex         = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr            source_name = lex->source_name;
        mp_parse_tree_t parse_tree  = mp_parse(lex, input_kind);
        mp_obj_t        module_fun  = mp_compile(&parse_tree, source_name, true);

        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t) nlr.ret_val);
    }
}

#endif


#if MICROPY_ENABLE_GC

static char *stack_top;
// TODO: set to proper value for our MCU
static char  heap[192 * 1024];

#endif


int main(int argc, char **argv) {
    cy_rslt_t result;

#if defined (CY_DEVICE_SECURE)

    cyhal_wdt_t wdt_obj;

    /* Clear watchdog timer so that it doesn't trigger a reset */
    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    cyhal_wdt_free(&wdt_obj);

#endif /* #if defined (CY_DEVICE_SECURE) */

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    
    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    // /* Enable global interrupts */
    // __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* retarget-io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }


    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("****************** "
           "HAL: Hello World! Example "
           "****************** \r\n\n");
    printf("Hello World!!!\r\n\n");

    setvbuf( stdin,  NULL, _IONBF, 0 );
    setvbuf( stdout, NULL, _IONBF, 0 );

    int stack_dummy;
    stack_top = (char *)&stack_dummy;

    #if MICROPY_ENABLE_GC

    gc_init(heap, heap + sizeof(heap));

    #endif

    mp_init();
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_lib));
    readline_init0();

    #if MICROPY_ENABLE_COMPILER

    #if MICROPY_REPL_EVENT_DRIVEN

    pyexec_event_repl_init();

    for (;;) {
        int c = mp_hal_stdin_rx_chr();

        int ret = pyexec_event_repl_process_char(c);
        // printf("ret = %i\n", ret);
        // printf("c   = %i\n", c);

        if( ret != 0 ) {
            // printf("EOF!!!   %i   %i\n", c, ret);
            pyexec_event_repl_init();
        }

        // if( (ret = pyexec_event_repl_process_char(c))) {
        //     break;
        // }
    }

    #else

    printf("running do_str ...\r\n");
    do_str("print('hello world!', list(x+1 for x in range(10)), end='\\r\\n')", MP_PARSE_SINGLE_INPUT);

    printf("\r\nrunning do_str ...\r\n");
    do_str("for i in range(10):\r\n  print(i)", MP_PARSE_FILE_INPUT);

    printf("\r\nrunning pyexec_frozen_module on frozentest.py ...\r\n");
    pyexec_frozen_module("frozentest.py");

    printf("\r\nrunning pyexec_friendly_repl ...\r\n");
    pyexec_friendly_repl();

    #endif

    #else

    #endif

    mp_deinit();
    return 0;
}



// // extern unsigned int __bss_start__, __bss_end__, __data_start__, __data_end__, __HeapLimit, __StackLimit, __StackTop, __etext;
// // extern uint32_t __bss_start__, __bss_end__, __data_start__, __data_end__, __HeapLimit, __StackLimit, __StackTop, __etext;
// // #define CY_CPU_VTOR_ADDR            0xE000ED08
// #define __STARTUP_COPY_MULTIPLE       (1)
// #define __STARTUP_CLEAR_BSS_MULTIPLE  (1)

// void Reset_Handler(void) __attribute__((naked));

// // ResetHandler stolen from
// //    boards/CY8CPROTO-062-4343W/bsps/TARGET_APP_CY8CPROTO-062-4343W/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm4.S
// void Reset_Handler(void) {
//     __asm volatile ("bl Cy_OnResetUser");
//     __asm volatile ("cpsid i");

// /*  Firstly it copies data from read only memory to RAM. There are two schemes
//  *  to copy. One can copy more than one sections. Another can only copy
//  *  one section.  The former scheme needs more instructions and read-only
//  *  data to implement than the latter.
//  *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

// #ifdef __STARTUP_COPY_MULTIPLE
// /*  Multiple sections scheme.
//  *
//  *  Between symbol address __copy_table_start__ and __copy_table_end__,
//  *  there are array of triplets, each of which specify:
//  *    offset 0: LMA of start of a section to copy from
//  *    offset 4: VMA of start of a section to copy to
//  *    offset 8: size of the section to copy. Must be multiply of 4
//  *
//  *  All addresses must be aligned to 4 bytes boundary.
//  */
//     __asm volatile ("ldr    r4, =__copy_table_start__");
//     __asm volatile ("ldr    r5, =__copy_table_end__");

// __asm volatile (".L_loop0:");
//     __asm volatile ("cmp    r4, r5");
//     __asm volatile ("bge    .L_loop0_done");
//     __asm volatile ("ldr    r1, [r4]");
//     __asm volatile ("ldr    r2, [r4, #4]");
//     __asm volatile ("ldr    r3, [r4, #8]");

// __asm volatile (".L_loop0_0:");
//     __asm volatile ("subs    r3, #4");
//     __asm volatile ("ittt    ge");
//     __asm volatile ("ldrge    r0, [r1, r3]");
//     __asm volatile ("strge    r0, [r2, r3]");
//     __asm volatile ("bge    .L_loop0_0");

//     __asm volatile ("adds    r4, #12");
//     __asm volatile ("b    .L_loop0");

// __asm volatile (".L_loop0_done:");
// #else
// /*  Single section scheme.
//  *
//  *  The ranges of copy from/to are specified by following symbols
//  *    __etext: LMA of start of the section to copy from. Usually end of text
//  *    __data_start__: VMA of start of the section to copy to
//  *    __data_end__: VMA of end of the section to copy to
//  *
//  *  All addresses must be aligned to 4 bytes boundary.
//  */
//     __asm volatile ("ldr    r1, =__etext");
//     __asm volatile ("ldr    r2, =__data_start__");
//     __asm volatile ("ldr    r3, =__data_end__");

//     __asm volatile (".L_loop1:");
//     __asm volatile ("cmp    r2, r3");
//     __asm volatile ("ittt    lt");
//     __asm volatile ("ldrlt    r0, [r1], #4");
//     __asm volatile ("strlt    r0, [r2], #4");
//     __asm volatile ("blt    .L_loop1");
// #endif /*__STARTUP_COPY_MULTIPLE */

// /*  This part of work usually is done in C library startup code. Otherwise,
//  *  define this macro to enable it in this startup.
//  *
//  *  There are two schemes too. One can clear multiple BSS sections. Another
//  *  can only clear one section. The former is more size expensive than the
//  *  latter.
//  *
//  *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
//  *  Otherwise define macro __STARTUP_CLEAR_BSS to choose the later.
//  */
// #ifdef __STARTUP_CLEAR_BSS_MULTIPLE
// /*  Multiple sections scheme.
//  *
//  *  Between symbol address __copy_table_start__ and __copy_table_end__,
//  *  there are array of tuples specifying:
//  *    offset 0: Start of a BSS section
//  *    offset 4: Size of this BSS section. Must be multiply of 4
//  */
//     __asm volatile ("ldr    r3, =__zero_table_start__");
//     __asm volatile ("ldr    r4, =__zero_table_end__");

// __asm volatile (".L_loop2:");
//     __asm volatile ("cmp    r3, r4");
//     __asm volatile ("bge    .L_loop2_done");
//     __asm volatile ("ldr    r1, [r3]");
//     __asm volatile ("ldr    r2, [r3, #4]");
//     __asm volatile ("movs    r0, 0");

// __asm volatile (".L_loop2_0:");
//     __asm volatile ("subs    r2, #4");
//     __asm volatile ("itt    ge");
//     __asm volatile ("strge    r0, [r1, r2]");
//     __asm volatile ("bge    .L_loop2_0");

//     __asm volatile ("adds    r3, #8");
//     __asm volatile ("b    .L_loop2");
// __asm volatile (".L_loop2_done:");
// #elif defined (__STARTUP_CLEAR_BSS)
// /*  Single BSS section scheme.
//  *
//  *  The BSS section is specified by following symbols
//  *    __bss_start__: start of the BSS section.
//  *    __bss_end__: end of the BSS section.
//  *
//  *  Both addresses must be aligned to 4 bytes boundary.
//  */
//     __asm volatile ("ldr    r1, =__bss_start__");
//     __asm volatile ("ldr    r2, =__bss_end__");

//     __asm volatile ("movs    r0, 0");
// __asm volatile (".L_loop3:");
//     __asm volatile ("cmp    r1, r2");
//     __asm volatile ("itt    lt");
//     __asm volatile ("strlt    r0, [r1], #4");
//     __asm volatile ("blt    .L_loop3");
// #endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

//     /* Update Vector Table Offset Register. */
//     __asm volatile ("ldr r0, =__ramVectors");
//     __asm volatile ("ldr r1, =0xE000ED08");
// //    __asm volatile ("ldr r1, =CY_CPU_VTOR_ADDR");
//     __asm volatile ("str r0, [r1]");
//     __asm volatile ("dsb 0xF");

//     /* Enable the FPU if used */
//     __asm volatile ("bl Cy_SystemInitFpuEnable");

// #ifndef __NO_SYSTEM_INIT
//     __asm volatile ("bl    SystemInit");
// #endif

//     /* OS-specific low-level initialization */
//     __asm volatile ("bl    cy_toolchain_init");

//     /* Call C/C++ static constructors */
//     __asm volatile ("bl    __libc_init_array");

//     /* Execute main application */
//     __asm volatile ("bl    main");

//     /* Call C/C++ static destructors */
//     __asm volatile ("bl    __libc_fini_array");

//     /* Should never get here */
//     __asm volatile ("b   .");

//     __asm volatile (".pool");
//     __asm volatile (".size    Reset_Handler, . - Reset_Handler");
 
// //    main(0, NULL);
// }


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
