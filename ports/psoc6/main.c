
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
// cyhal_gpio_callback_data_t cb_data =
// {
//     .callback = gpio_interrupt_handler,
//     .callback_arg = NULL
// };


#if MICROPY_ENABLE_GC

extern uint8_t __StackTop, __StackLimit;

__attribute__((section(".bss"))) static char gc_heap[MICROPY_GC_HEAP_SIZE];

#endif


extern void rtc_init(void);
extern void time_init(void);
extern void os_init(void);
extern void machine_init(void);
extern void machine_deinit(void);

void mpy_task(void *arg);

TaskHandle_t mpy_task_handle;

#define MPY_TASK_STACK_SIZE                    (4096u)
#define MPY_TASK_PRIORITY                      (3u)

// extern void mod_network_lwip_init(void);
// bool is_retarget_io_initialized = false;
// bool is_led_initialized = false;


int main(int argc, char **argv) {

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

    // retarget-io init failed. Stop program execution
    if (result != CY_RSLT_SUCCESS) {
        mp_raise_ValueError(MP_ERROR_TEXT("cy_retarget_io_init failed !\n"));
    }

    // is_retarget_io_initialized = true;

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


    // result = cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    // error_handler(result, NULL);
    // is_led_initialized = true;

    // result = cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
    // error_handler(result, NULL);

    // /* Configure GPIO interrupt. */
    // cyhal_gpio_register_callback(CYBSP_USER_BTN, &cb_data);
    // cyhal_gpio_enable_event(CYBSP_USER_BTN, CYHAL_GPIO_IRQ_FALL, GPIO_INTERRUPT_PRIORITY, true);

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
    xTaskCreate(mpy_task, "MicroPython task", MPY_TASK_STACK_SIZE, NULL, MPY_TASK_PRIORITY, &mpy_task_handle);

    // /* Start the FreeRTOS scheduler. */
    vTaskStartScheduler();


    printf("Done !!\n");
    fflush(stdout);

    /* Should never get here. */
    CY_ASSERT(0);
    return 0;
}

// /* This variable holds the value of the total number of the scan results that is
//  * available in the scan callback function in the current scan.
//  */
// uint32_t num_scan_result;


// /* This is used to decide the type of scan filter to be used. The valid values
//  * are provided in the enumeration scan_filter_mode.
//  */
// enum scan_filter_mode scan_filter_mode_select = SCAN_FILTER_NONE;

// const char *band_string[] =
// {
//     [CY_WCM_WIFI_BAND_ANY] = "2.4 and 5 GHz",
//     [CY_WCM_WIFI_BAND_2_4GHZ] = "2.4 GHz",
//     [CY_WCM_WIFI_BAND_5GHZ] = "5 GHz"
// };

// static void print_scan_result(cy_wcm_scan_result_t *result) {
//     char *security_type_string;

//     /* Convert the security type of the scan result to the corresponding
//      * security string
//      */
//     switch (result->security)
//     {
//         case CY_WCM_SECURITY_OPEN:
//             security_type_string = SECURITY_OPEN;
//             break;
//         case CY_WCM_SECURITY_WEP_PSK:
//             security_type_string = SECURITY_WEP_PSK;
//             break;
//         case CY_WCM_SECURITY_WEP_SHARED:
//             security_type_string = SECURITY_WEP_SHARED;
//             break;
//         case CY_WCM_SECURITY_WPA_TKIP_PSK:
//             security_type_string = SECURITY_WEP_TKIP_PSK;
//             break;
//         case CY_WCM_SECURITY_WPA_AES_PSK:
//             security_type_string = SECURITY_WPA_AES_PSK;
//             break;
//         case CY_WCM_SECURITY_WPA_MIXED_PSK:
//             security_type_string = SECURITY_WPA_MIXED_PSK;
//             break;
//         case CY_WCM_SECURITY_WPA2_AES_PSK:
//             security_type_string = SECURITY_WPA2_AES_PSK;
//             break;
//         case CY_WCM_SECURITY_WPA2_TKIP_PSK:
//             security_type_string = SECURITY_WPA2_TKIP_PSK;
//             break;
//         case CY_WCM_SECURITY_WPA2_MIXED_PSK:
//             security_type_string = SECURITY_WPA2_MIXED_PSK;
//             break;
//         case CY_WCM_SECURITY_WPA2_FBT_PSK:
//             security_type_string = SECURITY_WPA2_FBT_PSK;
//             break;
//         case CY_WCM_SECURITY_WPA3_SAE:
//             security_type_string = SECURITY_WPA3_SAE;
//             break;
//         case CY_WCM_SECURITY_WPA3_WPA2_PSK:
//             security_type_string = SECURITY_WPA3_WPA2_PSK;
//             break;
//         case CY_WCM_SECURITY_IBSS_OPEN:
//             security_type_string = SECURITY_IBSS_OPEN;
//             break;
//         case CY_WCM_SECURITY_WPS_SECURE:
//             security_type_string = SECURITY_WPS_SECURE;
//             break;
//         case CY_WCM_SECURITY_UNKNOWN:
//             security_type_string = SECURITY_UNKNOWN;
//             break;
//         case CY_WCM_SECURITY_WPA2_WPA_AES_PSK:
//             security_type_string = SECURITY_WPA2_WPA_AES_PSK;
//             break;
//         case CY_WCM_SECURITY_WPA2_WPA_MIXED_PSK:
//             security_type_string = SECURITY_WPA2_WPA_MIXED_PSK;
//             break;
//         case CY_WCM_SECURITY_WPA_TKIP_ENT:
//             security_type_string = SECURITY_WPA_TKIP_ENT;
//             break;
//         case CY_WCM_SECURITY_WPA_AES_ENT:
//             security_type_string = SECURITY_WPA_AES_ENT;
//             break;
//         case CY_WCM_SECURITY_WPA_MIXED_ENT:
//             security_type_string = SECURITY_WPA_MIXED_ENT;
//             break;
//         case CY_WCM_SECURITY_WPA2_TKIP_ENT:
//             security_type_string = SECURITY_WPA2_TKIP_ENT;
//             break;
//         case CY_WCM_SECURITY_WPA2_AES_ENT:
//             security_type_string = SECURITY_WPA2_AES_ENT;
//             break;
//         case CY_WCM_SECURITY_WPA2_MIXED_ENT:
//             security_type_string = SECURITY_WPA2_MIXED_ENT;
//             break;
//         case CY_WCM_SECURITY_WPA2_FBT_ENT:
//             security_type_string = SECURITY_WPA2_FBT_ENT;
//             break;
//         default:
//             security_type_string = SECURITY_UNKNOWN;
//             break;
//     }

//     printf(" %2" PRIu32 "   %-32s     %4d     %2d      %02X:%02X:%02X:%02X:%02X:%02X         %-15s\n",
//         num_scan_result, result->SSID,
//         result->signal_strength, result->channel, result->BSSID[0], result->BSSID[1],
//         result->BSSID[2], result->BSSID[3], result->BSSID[4], result->BSSID[5],
//         security_type_string);
// }

#define TO_KB(size_bytes)  ((float)(size_bytes) / 1024)


/*******************************************************************************
 * Function Definitions
 ******************************************************************************/

/*******************************************************************************
* Function Name: print_heap_usage
********************************************************************************
* Summary:
* Prints the available heap and utilized heap by using mallinfo().
*
*******************************************************************************/
void print_heap_usage(char *msg) {
    /* ARM compiler also defines __GNUC__ */
    #if defined(PRINT_HEAP_USAGE) && defined(__GNUC__) && !defined(__ARMCC_VERSION)
    struct mallinfo mall_info = mallinfo();

    extern uint8_t __HeapBase;  /* Symbol exported by the linker. */
    extern uint8_t __HeapLimit; /* Symbol exported by the linker. */

    uint8_t *heap_base = (uint8_t *)&__HeapBase;
    uint8_t *heap_limit = (uint8_t *)&__HeapLimit;
    uint32_t heap_size = (uint32_t)(heap_limit - heap_base);

    printf("\r\n\n********** Heap Usage **********\r\n");
    printf(msg);
    printf("\r\nTotal available heap        : %" PRIu32 " bytes/%.2f KB\r\n", heap_size, TO_KB(heap_size));

    printf("Maximum heap utilized so far: %u bytes/%.2f KB, %.2f%% of available heap\r\n",
        mall_info.arena, TO_KB(mall_info.arena), ((float)mall_info.arena * 100u) / heap_size);

    printf("Heap in use at this point   : %u bytes/%.2f KB, %.2f%% of available heap\r\n",
        mall_info.uordblks, TO_KB(mall_info.uordblks), ((float)mall_info.uordblks * 100u) / heap_size);

    printf("********************************\r\n\n");
    #endif /* #if defined(PRINT_HEAP_USAGE) && defined (__GNUC__) && !defined(__ARMCC_VERSION) */
}

// void gpio_interrupt_handler(void *handler_arg, cyhal_gpio_event_t event) {
//     /* Increment and check if the scan filter selected is invalid. If invalid,
//      * reset to no scan filter.
//      */
//     if (++scan_filter_mode_select >= SCAN_FILTER_INVALID) {
//         scan_filter_mode_select = SCAN_FILTER_NONE;
//     }
// }

// static void scan_callback(cy_wcm_scan_result_t *result_ptr, void *user_data, cy_wcm_scan_status_t status) {
//     if ((strlen((const char *)result_ptr->SSID) != 0) && (status == CY_WCM_SCAN_INCOMPLETE)) {
//         num_scan_result++;
//         print_scan_result(result_ptr);
//     }

//     if ((CY_WCM_SCAN_COMPLETE == status)) {
//         /* Reset the number of scan results to 0 for the next scan.*/
//         num_scan_result = 0;

//         /* Notify that scan has completed.*/
//         xTaskNotify(mpy_task_handle, 0, eNoAction);
//     }
// }

// void error_handler(cy_rslt_t result, char *message) {
//     if (CY_RSLT_SUCCESS != result) {
//         if (is_led_initialized) {
//             cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_ON);
//         }

//         if (is_retarget_io_initialized && (NULL != message)) {
//             ERR_INFO(("%s", message));
//         }

//         __disable_irq();
//         CY_ASSERT(0);
//     }
// }

void mpy_task(void *arg) {
    cy_wcm_scan_filter_t scan_filter;
    // cy_rslt_t result = CY_RSLT_SUCCESS;
    cy_wcm_config_t wcm_config = { .interface = CY_WCM_INTERFACE_TYPE_STA };
    // cy_wcm_mac_t scan_for_mac_value = {SCAN_FOR_MAC_ADDRESS};

    memset(&scan_filter, 0, sizeof(cy_wcm_scan_filter_t));
    // result =
    cy_wcm_init(&wcm_config);
    // error_handler(result, "Failed to initialize Wi-Fi Connection Manager.\n");

    // int i = 0;
    // // while (true)
    // while (i++ < 5) {
    //     /* Select the type of filter to use.*/
    //     switch (scan_filter_mode_select)
    //     {
    //         case SCAN_FILTER_NONE:
    //             APP_INFO(("Scanning without any filter\n"));
    //             break;

    //         case SCAN_FILTER_SSID:
    //             APP_INFO(("Scanning for %s.\n", SCAN_FOR_SSID_VALUE));

    //             /* Configure the scan filter for SSID specified by
    //              * SCAN_FOR_SSID_VALUE.
    //              */
    //             scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_SSID;
    //             memcpy(scan_filter.param.SSID, SCAN_FOR_SSID_VALUE, sizeof(SCAN_FOR_SSID_VALUE));
    //             break;

    //         case SCAN_FILTER_RSSI:
    //             APP_INFO(("Scanning for RSSI > %d dBm.\n", SCAN_FOR_RSSI_VALUE));

    //             /* Configure the scan filter for RSSI range specified by
    //              * SCAN_FOR_RSSI_VALUE.
    //              */
    //             scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_RSSI;
    //             scan_filter.param.rssi_range = SCAN_FOR_RSSI_VALUE;
    //             break;

    //         case SCAN_FILTER_MAC:
    //             APP_INFO(("Scanning for %02X:%02X:%02X:%02X:%02X:%02X.\n", scan_for_mac_value[0], scan_for_mac_value[1], scan_for_mac_value[2], scan_for_mac_value[3], scan_for_mac_value[4], scan_for_mac_value[5]));

    //             /* Configure the scan filter for MAC specified by scan_for_mac_value
    //              */
    //             scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_MAC;
    //             memcpy(scan_filter.param.BSSID, &scan_for_mac_value, sizeof(scan_for_mac_value));
    //             break;

    //         case SCAN_FILTER_BAND:
    //             APP_INFO(("Scanning in %s band.\n", band_string[SCAN_FOR_BAND_VALUE]));

    //             /* Configure the scan filter for band specified by
    //              * SCAN_FOR_BAND_VALUE.
    //              */
    //             scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_BAND;
    //             scan_filter.param.band = SCAN_FOR_BAND_VALUE;
    //             break;

    //         default:
    //             break;
    //     }

    //     PRINT_SCAN_TEMPLATE();

    //     if (SCAN_FILTER_NONE == scan_filter_mode_select) {
    //         result = cy_wcm_start_scan(scan_callback, NULL, NULL);
    //     } else {
    //         result = cy_wcm_start_scan(scan_callback, NULL, &scan_filter);
    //     }

    //     /* Wait for scan completion if scan was started successfully. The API
    //      * cy_wcm_start_scan doesn't wait for scan completion. If it is called
    //      * again when the scan hasn't completed, the API returns
    //      * CY_WCM_RESULT_SCAN_IN_PROGRESS.
    //      */
    //     if (CY_RSLT_SUCCESS == result) {
    //         xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
    //     }

    //     /* Define PRINT_HEAP_USAGE using DEFINES variable in the Makefile. */
    //     print_heap_usage("After scan results are printed to UART");

    //     vTaskDelay(pdMS_TO_TICKS(SCAN_DELAY_MS));
    // }


    // fflush(stdout);

soft_reset:

    mp_init();

    // ANSI ESC sequence for clear screen. Refer to  https://stackoverflow.com/questions/517970/how-to-clear-the-interpreter-console
    // mp_printf(&mp_plat_print, "\033[H\033[2J");

    mp_printf(&mp_plat_print, MICROPY_BANNER_NAME_AND_VERSION);
    mp_printf(&mp_plat_print, "; " MICROPY_BANNER_MACHINE);
    mp_printf(&mp_plat_print, "\nUse Ctrl-D to exit, Ctrl-E for paste mode\n");

    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_flash));
    // mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_flash_slash_lib));

    // indicate in REPL console when debug mode is selected
    mplogger_print("\n...LOGGER DEBUG MODE...\n\n");

    readline_init0();
    machine_init();


    #if MICROPY_VFS_FAT
    pyexec_frozen_module("vfs_fat.py");
    #elif MICROPY_VFS_LFS2
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

    mp_printf(&mp_plat_print, "MPY: soft reboot\n");

    // Deinitialize modules
    machine_deinit();

    // gc_sweep_all();
    mp_deinit();

    goto soft_reset;
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
