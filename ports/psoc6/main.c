
// std includes
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>


// micropython includes
 #include "py/builtin.h"
#include "py/mpconfig.h"
#include "py/mperrno.h"
#include "shared/readline/readline.h"
#include "shared/runtime/pyexec.h"


#include "extmod/vfs.h"



#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>

#include "py/compile.h"
#include "py/compile.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/objstr.h"
#include "py/stackctrl.h"
#include "py/mphal.h"
#include "py/mpthread.h"
#include "extmod/misc.h"
#include "extmod/moduplatform.h"
#include "genhdr/mpversion.h"

// MTB includes
#include "cybsp.h"
#include "cyhal.h"
#include "cy_retarget_io.h"



extern int mp_hal_stdin_rx_chr(void);
extern void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len);


#if MICROPY_ENABLE_COMPILER

void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    printf("do_str\n");

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
// STATIC void print_help(char **argv) {
//     printf(
//         "usage: %s [<opts>] [-X <implopt>] [-c <command> | -m <module> | <filename>]\n"
//         "Options:\n"
//         "-h : print this help message\n"
//         "-i : enable inspection via REPL after running command/module/file\n"
//         #if MICROPY_DEBUG_PRINTERS
//         "-v : verbose (trace various operations); can be multiple\n"
//         #endif
//         "-O[N] : apply bytecode optimizations of level N\n"
//         "\n"
//         "Implementation specific options (-X):\n", argv[0]
//         );
//     int impl_opts_cnt = 0;
//     printf(
//         "  compile-only                 -- parse and compile only\n"
//         #if MICROPY_EMIT_NATIVE
//         "  emit={bytecode,native,viper} -- set the default code emitter\n"
//         #else
//         "  emit=bytecode                -- set the default code emitter\n"
//         #endif
//         );
//     impl_opts_cnt++;
//     // #if MICROPY_ENABLE_GC
//     // printf(
//     //     "  heapsize=<n>[w][K|M] -- set the heap size for the GC (default %ld)\n"
//     //     , heap_size);
//     // impl_opts_cnt++;
//     // #endif
//     #if defined(__APPLE__)
//     printf("  realtime -- set thread priority to realtime\n");
//     impl_opts_cnt++;
//     #endif

//     if (impl_opts_cnt == 0) {
//         printf("  (none)\n");
//     }
// }

// STATIC int invalid_args(void) {
//     fprintf(stderr, "Invalid command line arguments. Use -h option for help.\n");
//     return 1;
// }

// // Process options which set interpreter init options
// STATIC void pre_process_options(int argc, char **argv) {
//     for (int a = 1; a < argc; a++) {
//         if (argv[a][0] == '-') {
//             if (strcmp(argv[a], "-c") == 0 || strcmp(argv[a], "-m") == 0) {
//                 break; // Everything after this is a command/module and arguments for it
//             }
//             if (strcmp(argv[a], "-h") == 0) {
//                 print_help(argv);
//                 exit(0);
//             }
//             if (strcmp(argv[a], "-X") == 0) {
//                 if (a + 1 >= argc) {
//                     exit(invalid_args());
//                 }
//                 if (0) {
//                 // } else if (strcmp(argv[a + 1], "compile-only") == 0) {
//                 //     compile_only = true;
//                 // } else if (strcmp(argv[a + 1], "emit=bytecode") == 0) {
//                 //     emit_opt = MP_EMIT_OPT_BYTECODE;
//                 // #if MICROPY_EMIT_NATIVE
//                 // } else if (strcmp(argv[a + 1], "emit=native") == 0) {
//                 //     emit_opt = MP_EMIT_OPT_NATIVE_PYTHON;
//                 // } else if (strcmp(argv[a + 1], "emit=viper") == 0) {
//                 //     emit_opt = MP_EMIT_OPT_VIPER;
//                 // #endif
//                 // #if MICROPY_ENABLE_GC
//                 // } else if (strncmp(argv[a + 1], "heapsize=", sizeof("heapsize=") - 1) == 0) {
//                 //     char *end;
//                 //     heap_size = strtol(argv[a + 1] + sizeof("heapsize=") - 1, &end, 0);
//                 //     // Don't bring unneeded libc dependencies like tolower()
//                 //     // If there's 'w' immediately after number, adjust it for
//                 //     // target word size. Note that it should be *before* size
//                 //     // suffix like K or M, to avoid confusion with kilowords,
//                 //     // etc. the size is still in bytes, just can be adjusted
//                 //     // for word size (taking 32bit as baseline).
//                 //     bool word_adjust = false;
//                 //     if ((*end | 0x20) == 'w') {
//                 //         word_adjust = true;
//                 //         end++;
//                 //     }
//                 //     if ((*end | 0x20) == 'k') {
//                 //         heap_size *= 1024;
//                 //     } else if ((*end | 0x20) == 'm') {
//                 //         heap_size *= 1024 * 1024;
//                 //     } else {
//                 //         // Compensate for ++ below
//                 //         --end;
//                 //     }
//                 //     if (*++end != 0) {
//                 //         goto invalid_arg;
//                 //     }
//                 //     if (word_adjust) {
//                 //         heap_size = heap_size * MP_BYTES_PER_OBJ_WORD / 4;
//                 //     }
//                 //     // If requested size too small, we'll crash anyway
//                 //     if (heap_size < 700) {
//                 //         goto invalid_arg;
//                 //     }
//                 // #endif
//                 // #if defined(__APPLE__)
//                 // } else if (strcmp(argv[a + 1], "realtime") == 0) {
//                 //     #if MICROPY_PY_THREAD
//                 //     mp_thread_is_realtime_enabled = true;
//                 //     #endif
//                 //     // main thread was already intialized before the option
//                 //     // was parsed, so we have to enable realtime here.
//                 //     mp_thread_set_realtime();
//                 // #endif
//                 // } else {
//                 // invalid_arg:
//                 //     exit(invalid_args());
//                 }
//                 a++;
//             }
//         } else {
//             break; // Not an option but a file
//         }
//     }
// }

// STATIC void set_sys_argv(char *argv[], int argc, int start_arg) {
//     for (int i = start_arg; i < argc; i++) {
//         mp_obj_list_append(mp_sys_argv, MP_OBJ_NEW_QSTR(qstr_from_str(argv[i])));
//     }
// }

#if MICROPY_PY_SYS_EXECUTABLE
extern mp_obj_str_t mp_sys_executable_obj;
STATIC char executable_path[MICROPY_ALLOC_PATH_MAX];

STATIC void sys_set_excecutable(char *argv0) {
    if (realpath(argv0, executable_path)) {
        mp_obj_str_set_data(&mp_sys_executable_obj, (byte *)executable_path, strlen(executable_path));
    }
}
#endif


#if MICROPY_ENABLE_GC

static char *stack_top;
// TODO: set to proper value for our MCU
static char  heap[192 * 1024];

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

    mp_hal_stdout_tx_str(MICROPY_BANNER_NAME_AND_VERSION);
    mp_hal_stdout_tx_str("; " MICROPY_BANNER_MACHINE);
    mp_hal_stdout_tx_str("\nUse Ctrl-D to exit, Ctrl-E for paste mode\n");

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("****************** "
           "HAL: Hello World! Example "
           "****************** \r\n\n");

    printf("Hello World!!!\r\n\n");

    setvbuf( stdin,  NULL, _IONBF, 0 );
    setvbuf( stdout, NULL, _IONBF, 0 );


    cyhal_flash_t      flash_obj;
    cyhal_flash_info_t flash_info;
    // uint32_t           flash_start_address;
    // uint32_t           flash_size;
    // uint32_t           flash_sector_size;
    // uint32_t           flash_page_size;
    // uint8_t            flash_erase_value;
 
    // Initialize flash object
    cyhal_flash_init(&flash_obj);
 
    // Get flash characteristics
    cyhal_flash_get_info(&flash_obj, &flash_info);
 
    // Wait for 100ms for the flash write to complete
    uint32_t timeout = 100;
 
    // Wait for the command to finish execution
    while ((true != cyhal_flash_is_operation_complete(&flash_obj)) && (0 < timeout))
    {
        timeout--;
        cyhal_system_delay_ms(1); // delay one millisecond each iteration
    }
 
    const cyhal_flash_block_info_t* block_info = 0;

    if (0 != timeout)
    {
        for (int index = 0; index < flash_info.block_count; index++)
        {
            block_info = flash_info.blocks;
            block_info         += index;
            // flash_start_address = block_info->start_address;
            // flash_size          = block_info->size;
            // flash_sector_size   = block_info->sector_size;
            // flash_page_size     = block_info->page_size;
            // flash_erase_value   = block_info->erase_value;


            printf("block_info->start_address : %lx\n", block_info->start_address);
            printf("block_info->size : %lx\n", block_info->size);
            printf("block_info->sector_size : %lx\n", block_info->sector_size);
            printf("block_info->page_size : %lx\n", block_info->page_size);
            printf("block_info->erase_value : %x\n\n\n", block_info->erase_value);
        }
    }
 

{
   // Structure tm stores years since 1900
    #define TM_YEAR_BASE (1900u)
 
    cy_rslt_t   result;
    cyhal_rtc_t my_rtc;
    char        buffer[80];
 
    // int mday = 3, month = 3, year = 2020; // Day of month, month and year
    // int hours = 8, minutes = 10, seconds = 10; // Hours, minutes and seconds
    // int wday = 3; // Days from Sunday. Sunday is 0, Monday is 1 and so on.
    // int dst  = 0; // Daylight Savings. 0 - Disabled, 1 - Enabled
 
    // // Setting the tm structure as 08 HRS:10 MIN:10 SEC ; 3rd March 2020; Wednesday ; DST off
    // struct tm new_date_time =
    // {
    //     .tm_sec   = seconds,
    //     .tm_min   = minutes,
    //     .tm_hour  = hours,
    //     .tm_mday  = mday,
    //     .tm_mon   = month - 1,
    //     .tm_year  = year - TM_YEAR_BASE,
    //     .tm_wday  = wday,
    //     .tm_isdst = dst
    // };
    struct tm current_date_time = {0};
 
    // Initialize RTC
    result = cyhal_rtc_init(&my_rtc);
 
//    // Update the current time and date to the RTC peripheral
//    result = cyhal_rtc_write(&my_rtc, &new_date_time);
 
    // Get the current time and date from the RTC peripheral
    result = cyhal_rtc_read(&my_rtc, &current_date_time);
    if (CY_RSLT_SUCCESS == result)
    {
        // strftime() is a C library function which is used to format date and time into string.
        // It comes under the header file "time.h" which is included by HAL (See
        // http://www.cplusplus.com/reference/ctime/strftime/)
        strftime(buffer, sizeof(buffer), "%c", &current_date_time);
        // Print the buffer in serial terminal to view the current date and time
        printf("time : %s\n", buffer);
    }
    }


    // struct timespec ts;
    // clock_gettime(CLOCK_MONOTONIC, &ts);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    cyhal_trng_t trng_obj;
    uint32_t     rnd_num;
 
    // Initialize the true random number generator block
    result = cyhal_trng_init(&trng_obj);
 
    // Generate a true random number
    rnd_num = cyhal_trng_generate(&trng_obj);
    printf("random : %ld\n", rnd_num);
    // Release the true random number generator block
    // Note: Free only if not required anymore
    cyhal_trng_free(&trng_obj);
 

    int stack_dummy;

soft_reset:

    stack_top = (char *)&stack_dummy;

    #if MICROPY_ENABLE_GC

    gc_init(heap, heap + sizeof(heap));

    #endif

    #if MICROPY_ENABLE_PYSTACK
    static mp_obj_t pystack[1024];
    mp_pystack_init(pystack, &pystack[MP_ARRAY_SIZE(pystack)]);
    #endif

    mp_init();

    
    // #if MICROPY_VFS_POSIX
    // {
    //     static_assert(false);
    //     // Mount the host FS at the root of our internal VFS
    //     mp_obj_t args[2] = {
    //         MP_OBJ_TYPE_GET_SLOT(&mp_type_vfs_posix, make_new)(&mp_type_vfs_posix, 0, 0, NULL),
    //         MP_OBJ_NEW_QSTR(MP_QSTR__slash_),
    //     };
    //     mp_vfs_mount(2, args, (mp_map_t *)&mp_const_empty_map);
    //     MP_STATE_VM(vfs_cur) = MP_STATE_VM(vfs_mount_table);
    // }
    // #endif

    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_lib));
    readline_init0();

    // printf("running do_str ...\r\n");
    // do_str("print('hello world!', list(x+1 for x in range(10)), end='\\r\\n')", MP_PARSE_SINGLE_INPUT);

    // printf("\r\nrunning do_str ...\r\n");
    // do_str("for i in range(10):\r\n  print(i)", MP_PARSE_FILE_INPUT);

    // printf("\r\nrunning pyexec_frozen_module on frozentest.py ...\r\n");
    // // no directory prefix "freeze/" !
    // pyexec_frozen_module("frozentest.py");

    #if MICROPY_ENABLE_COMPILER

    // #if MICROPY_REPL_EVENT_DRIVEN

    // pyexec_event_repl_init();

//     for (;;) {
//         int c = mp_hal_stdin_rx_chr();

//         int ret = pyexec_event_repl_process_char(c);
//         // printf("ret = %i\n", ret);
//         // printf("c   = %i\n", c);

// //  if( c == CHAR_CTRL_D ) {
//         if( ret != 0 ) {
//             // printf("EOF!!!   %i   %i\n", c, ret);
//             pyexec_event_repl_init();
//         }

//         // if( (ret = pyexec_event_repl_process_char(c))) {
//         //     break;
//         // }
//     }

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
    printf("gc_collect\n");

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


// // TODO: to be implemented
// mp_import_stat_t mp_import_stat(const char *path) {
//     return MP_IMPORT_STAT_NO_EXIST;
// }


// TODO: to be implemented
void nlr_jump_fail(void *val) {
    printf("nlr_jump_fail\n");

    while (1) {
        ;
    }
}


// // TODO: to be implemented
// mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
//     return mp_const_none;
// }

// MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);


//#include "py/reader.h"
//#include "py/ringbuf.h"
//#include "py/stream.h"
//#include "extmod/misc.h"


// void mp_reader_new_file_from_fd(mp_reader_t *reader, int fd, bool close_fd) {
//     mp_reader_posix_t *rp = m_new_obj(mp_reader_posix_t);
//     rp->close_fd = close_fd;
//     rp->fd = fd;
//     MP_THREAD_GIL_EXIT();
//     int n = read(rp->fd, rp->buf, sizeof(rp->buf));
//     if (n == -1) {
//         if (close_fd) {
//             close(fd);
//         }
//         MP_THREAD_GIL_ENTER();
//         mp_raise_OSError(errno);
//     }
//     MP_THREAD_GIL_ENTER();
//     rp->len = n;
//     rp->pos = 0;
//     reader->data = rp;
//     reader->readbyte = mp_reader_posix_readbyte;
//     reader->close = mp_reader_posix_close;
// }


// void mp_reader_new_file(mp_reader_t *reader, const char *filename) {
//     printf("mp_reader_new_file\n");

//     MP_THREAD_GIL_EXIT();
//     int fd = open(filename, O_RDONLY, 0644);
//     MP_THREAD_GIL_ENTER();
//     if (fd < 0) {
//         mp_raise_OSError_with_filename(errno, filename);
//     }
//     mp_reader_new_file_from_fd(reader, fd, true);
// }


// const mp_obj_vfs_posix_file_t mp_sys_stdin_obj = {{&mp_type_vfs_posix_textio}, STDIN_FILENO};
// const mp_obj_vfs_posix_file_t mp_sys_stdout_obj = {{&mp_type_vfs_posix_textio}, STDOUT_FILENO};
// const mp_obj_vfs_posix_file_t mp_sys_stderr_obj = {{&mp_type_vfs_posix_textio}, STDERR_FILENO};
// #ifndef MICROPY_HW_STDIN_BUFFER_LEN
// #define MICROPY_HW_STDIN_BUFFER_LEN 512
// #endif

// STATIC uint8_t stdin_ringbuf_array[MICROPY_HW_STDIN_BUFFER_LEN];
// ringbuf_t stdin_ringbuf = { stdin_ringbuf_array, sizeof(stdin_ringbuf_array) };

// uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags) {
//     printf("mp_hal_stdio_poll\n");

//     uintptr_t ret = 0;
//     // #if MICROPY_HW_ENABLE_USBDEV
//     // poll_cdc_interfaces();
//     // #endif
//     // #if MICROPY_HW_ENABLE_UART_REPL || MICROPY_HW_ENABLE_USBDEV
//     if ((poll_flags & MP_STREAM_POLL_RD) && ringbuf_peek(&stdin_ringbuf) != -1) {
//         ret |= MP_STREAM_POLL_RD;
//     }
//     // #endif
//     // #if MICROPY_PY_OS_DUPTERM
// //    ret |= mp_uos_dupterm_poll(poll_flags);
//     // #endif
//     return ret;
// }