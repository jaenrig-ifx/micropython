
// std includes
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>


// MTB includes
#include "cybsp.h"
#include "cybsp_wifi.h"
#include "cy_retarget_io.h"
#include "cyhal.h"
#include "cy_wcm.h"

/* FreeRTOS header file */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>


// micropython includes
#include "genhdr/mpversion.h"
#include "py/gc.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "py/stackctrl.h"
#include "shared/readline/readline.h"
#include "shared/runtime/pyexec.h"
#include "extmod/modnetwork.h"

#if MICROPY_PY_LWIP
#include "lwip/init.h"
#include "lwip/apps/mdns.h"
#endif

// port-specific includes
#include "mplogger.h"

/* Task header files */
#include "scan_task.h"


/* Include serial flash library and QSPI memory configurations only for the
 * kits that require the Wi-Fi firmware to be loaded in external QSPI NOR flash.
 */
#if defined(CY_DEVICE_PSOC6A512K)
#include "cy_serial_flash_qspi.h"
#include "cycfg_qspi_memslot.h"
#endif


// /*******************************************************************************
//  * Macros
//  ******************************************************************************/
#define GPIO_INTERRUPT_PRIORITY             (7)


// /*******************************************************************************
//  * Global Variables
//  ******************************************************************************/
cyhal_gpio_callback_data_t cb_data =
{
    .callback = gpio_interrupt_handler,
    .callback_arg = NULL
};


#if MICROPY_ENABLE_GC

extern uint8_t __StackTop, __StackLimit;

__attribute__((section(".bss"))) static char gc_heap[MICROPY_GC_HEAP_SIZE];

#endif


extern void rtc_init(void);
extern void time_init(void);
extern void os_init(void);


// extern void mod_network_lwip_init(void);
bool is_retarget_io_initialized = false;
bool is_led_initialized = false;


int main(int argc, char **argv) {
    // #if MICROPY_PY_THREAD
    // mp_thread_init();
    // #endif


    #if MICROPY_ENABLE_GC

    mp_stack_set_top(&__StackTop);
    // mp_stack_set_limit((mp_uint_t)&__StackTop - (mp_uint_t)&__StackLimit);
    mp_stack_set_limit((mp_uint_t)&__StackLimit);
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


    // Initialize the device and board peripherals
    cy_rslt_t result = cybsp_init();

    // Board init failed. Stop program execution
    if (result != CY_RSLT_SUCCESS) {
        mp_raise_ValueError(MP_ERROR_TEXT("cybsp_init failed !\n"));
    }


    // Initialize retarget-io to use the debug UART port
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    error_handler(result, NULL);

    // retarget-io init failed. Stop program execution
    if (result != CY_RSLT_SUCCESS) {
        mp_raise_ValueError(MP_ERROR_TEXT("cy_retarget_io_init failed !\n"));
    }

    is_retarget_io_initialized = true;

    // Initialize modules. Or to be redone after a reset and therefore to be placed next to machine_init below ?
    os_init();
    rtc_init();
    time_init();


    #if MICROPY_PY_LWIP
    // lwIP doesn't allow to reinitialise itself by subsequent calls to this function
    // because the system timeout list (next_timeout) is only ever reset by BSS clearing.
    // So for now we only init the lwIP stack once on power-up.
    lwip_init();
    #if LWIP_MDNS_RESPONDER
    mdns_resp_init();
    #endif
    #endif


    result = cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    error_handler(result, NULL);
    is_led_initialized = true;

    result = cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
    error_handler(result, NULL);

    /* Configure GPIO interrupt. */
    cyhal_gpio_register_callback(CYBSP_USER_BTN, &cb_data);
    cyhal_gpio_enable_event(CYBSP_USER_BTN, CYHAL_GPIO_IRQ_FALL, GPIO_INTERRUPT_PRIORITY, true);

    // /* Enable global interrupts. */
    // __enable_irq();


    /* Init QSPI and enable XIP to get the Wi-Fi firmware from the QSPI NOR flash */
    #if defined(CY_DEVICE_PSOC6A512K)
    const uint32_t bus_frequency = 50000000lu;
    cy_serial_flash_qspi_init(smifMemConfigs[0], CYBSP_QSPI_D0, CYBSP_QSPI_D1,
        CYBSP_QSPI_D2, CYBSP_QSPI_D3, NC, NC, NC, NC,
        CYBSP_QSPI_SCK, CYBSP_QSPI_SS, bus_frequency);

    cy_serial_flash_qspi_enable_xip(true);
    #endif

    /* Create the tasks. */
    xTaskCreate(scan_task, "Scan task", SCAN_TASK_STACK_SIZE, NULL, SCAN_TASK_PRIORITY, &scan_task_handle);

    // /* Start the FreeRTOS scheduler. */
    vTaskStartScheduler();


    printf("Done !!\n");
    fflush(stdout);

    /* Should never get here. */
    CY_ASSERT(0);
    return 0;
}


// // TODO: to be implemented
void nlr_jump_fail(void *val) {
    mplogger_print("nlr_jump_fail\n");

    mp_printf(&mp_plat_print, "FATAL: uncaught exception %p\n", val);
    mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(val));

    for (;;) {
        __BKPT(0);
    }
}
