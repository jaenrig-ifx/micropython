/*******************************************************************************
* File Name: scan_task.c
*
* Description: This file contains functions that perform network related tasks
* like connecting to an AP, scanning for APs, connection event callbacks, and
* utility function for printing scan results.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2020-2022, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
 * Header file includes
 ******************************************************************************/
#include "cybsp.h"
#include "cyhal.h"
#include <inttypes.h>

/* FreeRTOS header file.*/
#include "FreeRTOS.h"

/*Wi-Fi Connection Manager header file.*/
#include "cy_wcm.h"

/*Task header file*/
#include "scan_task.h"


// micropython includes
#include "genhdr/mpversion.h"
#include "py/gc.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "py/stackctrl.h"
#include "shared/readline/readline.h"
#include "shared/runtime/pyexec.h"
#include "mplogger.h"

extern void machine_init(void);
extern void machine_deinit(void);


// extern void mod_network_lwip_init(void);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
TaskHandle_t scan_task_handle;

/* This variable holds the value of the total number of the scan results that is
 * available in the scan callback function in the current scan.
 */
uint32_t num_scan_result;

/* This is used to decide the type of scan filter to be used. The valid values
 * are provided in the enumeration scan_filter_mode.
 */
enum scan_filter_mode scan_filter_mode_select = SCAN_FILTER_NONE;

const char *band_string[] =
{
    [CY_WCM_WIFI_BAND_ANY] = "2.4 and 5 GHz",
    [CY_WCM_WIFI_BAND_2_4GHZ] = "2.4 GHz",
    [CY_WCM_WIFI_BAND_5GHZ] = "5 GHz"
};


/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
static void scan_callback(cy_wcm_scan_result_t *result_ptr, void *user_data, cy_wcm_scan_status_t status);
static void print_scan_result(cy_wcm_scan_result_t *result);
void print_heap_usage(char *msg);


/*******************************************************************************
 * Function Definitions
 ******************************************************************************/


/*******************************************************************************
 * Function Name: scan_task
 *******************************************************************************
 * Summary: This task initializes the Wi-Fi device, Wi-Fi transport, lwIP
 * network stack, and issues a scan for the available networks. A scan filter
 * is applied depending on the value of scan_filter_mode_select. After starting
 * the scan, it waits for the notification from the scan callback for completion
 * of the scan. It then waits for a delay specified by SCAN_DELAY_PERIOD_MS
 * before repeating the process.
 *
 *
 * Parameters:
 *  void* arg: Task parameter defined during task creation (unused).
 *
 * Return:
 *  void
 *
 ******************************************************************************/
void scan_task(void *arg) {
    cy_wcm_scan_filter_t scan_filter;
    cy_rslt_t result = CY_RSLT_SUCCESS;
    cy_wcm_config_t wcm_config = { .interface = CY_WCM_INTERFACE_TYPE_STA };
    cy_wcm_mac_t scan_for_mac_value = {SCAN_FOR_MAC_ADDRESS};

    memset(&scan_filter, 0, sizeof(cy_wcm_scan_filter_t));
    result = cy_wcm_init(&wcm_config);
    error_handler(result, "Failed to initialize Wi-Fi Connection Manager.\n");

    int i = 0;
    // while (true)
    while (i++ < 5) {
        /* Select the type of filter to use.*/
        switch (scan_filter_mode_select)
        {
            case SCAN_FILTER_NONE:
                APP_INFO(("Scanning without any filter\n"));
                break;

            case SCAN_FILTER_SSID:
                APP_INFO(("Scanning for %s.\n", SCAN_FOR_SSID_VALUE));

                /* Configure the scan filter for SSID specified by
                 * SCAN_FOR_SSID_VALUE.
                 */
                scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_SSID;
                memcpy(scan_filter.param.SSID, SCAN_FOR_SSID_VALUE, sizeof(SCAN_FOR_SSID_VALUE));
                break;

            case SCAN_FILTER_RSSI:
                APP_INFO(("Scanning for RSSI > %d dBm.\n", SCAN_FOR_RSSI_VALUE));

                /* Configure the scan filter for RSSI range specified by
                 * SCAN_FOR_RSSI_VALUE.
                 */
                scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_RSSI;
                scan_filter.param.rssi_range = SCAN_FOR_RSSI_VALUE;
                break;

            case SCAN_FILTER_MAC:
                APP_INFO(("Scanning for %02X:%02X:%02X:%02X:%02X:%02X.\n", scan_for_mac_value[0], scan_for_mac_value[1], scan_for_mac_value[2], scan_for_mac_value[3], scan_for_mac_value[4], scan_for_mac_value[5]));

                /* Configure the scan filter for MAC specified by scan_for_mac_value
                 */
                scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_MAC;
                memcpy(scan_filter.param.BSSID, &scan_for_mac_value, sizeof(scan_for_mac_value));
                break;

            case SCAN_FILTER_BAND:
                APP_INFO(("Scanning in %s band.\n", band_string[SCAN_FOR_BAND_VALUE]));

                /* Configure the scan filter for band specified by
                 * SCAN_FOR_BAND_VALUE.
                 */
                scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_BAND;
                scan_filter.param.band = SCAN_FOR_BAND_VALUE;
                break;

            default:
                break;
        }

        PRINT_SCAN_TEMPLATE();

        if (SCAN_FILTER_NONE == scan_filter_mode_select) {
            result = cy_wcm_start_scan(scan_callback, NULL, NULL);
        } else {
            result = cy_wcm_start_scan(scan_callback, NULL, &scan_filter);
        }

        /* Wait for scan completion if scan was started successfully. The API
         * cy_wcm_start_scan doesn't wait for scan completion. If it is called
         * again when the scan hasn't completed, the API returns
         * CY_WCM_RESULT_SCAN_IN_PROGRESS.
         */
        if (CY_RSLT_SUCCESS == result) {
            xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        }

        /* Define PRINT_HEAP_USAGE using DEFINES variable in the Makefile. */
        print_heap_usage("After scan results are printed to UART");

        vTaskDelay(pdMS_TO_TICKS(SCAN_DELAY_MS));
    }


    fflush(stdout);

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


/*******************************************************************************
 * Function Name: scan_callback
 *******************************************************************************
 * Summary: The callback function which accumulates the scan results. After
 * completing the scan, it sends a task notification to scan_task.
 *
 * Parameters:
 *  cy_wcm_scan_result_t *result_ptr: Pointer to the scan result
 *  void *user_data: User data.
 *  cy_wcm_scan_status_t status: Status of scan completion.
 *
 * Return:
 *  void
 *
 ******************************************************************************/
static void scan_callback(cy_wcm_scan_result_t *result_ptr, void *user_data, cy_wcm_scan_status_t status) {
    if ((strlen((const char *)result_ptr->SSID) != 0) && (status == CY_WCM_SCAN_INCOMPLETE)) {
        num_scan_result++;
        print_scan_result(result_ptr);
    }

    if ((CY_WCM_SCAN_COMPLETE == status)) {
        /* Reset the number of scan results to 0 for the next scan.*/
        num_scan_result = 0;

        /* Notify that scan has completed.*/
        xTaskNotify(scan_task_handle, 0, eNoAction);
    }
}


/*******************************************************************************
 * Function Name: print_scan_result
 *******************************************************************************
 * Summary: This function prints the scan result accumulated by the scan
 * handler.
 *
 *
 * Parameters:
 *  cy_wcm_scan_result_t *result: Pointer to the scan result.
 *
 * Return:
 *  void
 *
 ******************************************************************************/
static void print_scan_result(cy_wcm_scan_result_t *result) {
    char *security_type_string;

    /* Convert the security type of the scan result to the corresponding
     * security string
     */
    switch (result->security)
    {
        case CY_WCM_SECURITY_OPEN:
            security_type_string = SECURITY_OPEN;
            break;
        case CY_WCM_SECURITY_WEP_PSK:
            security_type_string = SECURITY_WEP_PSK;
            break;
        case CY_WCM_SECURITY_WEP_SHARED:
            security_type_string = SECURITY_WEP_SHARED;
            break;
        case CY_WCM_SECURITY_WPA_TKIP_PSK:
            security_type_string = SECURITY_WEP_TKIP_PSK;
            break;
        case CY_WCM_SECURITY_WPA_AES_PSK:
            security_type_string = SECURITY_WPA_AES_PSK;
            break;
        case CY_WCM_SECURITY_WPA_MIXED_PSK:
            security_type_string = SECURITY_WPA_MIXED_PSK;
            break;
        case CY_WCM_SECURITY_WPA2_AES_PSK:
            security_type_string = SECURITY_WPA2_AES_PSK;
            break;
        case CY_WCM_SECURITY_WPA2_TKIP_PSK:
            security_type_string = SECURITY_WPA2_TKIP_PSK;
            break;
        case CY_WCM_SECURITY_WPA2_MIXED_PSK:
            security_type_string = SECURITY_WPA2_MIXED_PSK;
            break;
        case CY_WCM_SECURITY_WPA2_FBT_PSK:
            security_type_string = SECURITY_WPA2_FBT_PSK;
            break;
        case CY_WCM_SECURITY_WPA3_SAE:
            security_type_string = SECURITY_WPA3_SAE;
            break;
        case CY_WCM_SECURITY_WPA3_WPA2_PSK:
            security_type_string = SECURITY_WPA3_WPA2_PSK;
            break;
        case CY_WCM_SECURITY_IBSS_OPEN:
            security_type_string = SECURITY_IBSS_OPEN;
            break;
        case CY_WCM_SECURITY_WPS_SECURE:
            security_type_string = SECURITY_WPS_SECURE;
            break;
        case CY_WCM_SECURITY_UNKNOWN:
            security_type_string = SECURITY_UNKNOWN;
            break;
        case CY_WCM_SECURITY_WPA2_WPA_AES_PSK:
            security_type_string = SECURITY_WPA2_WPA_AES_PSK;
            break;
        case CY_WCM_SECURITY_WPA2_WPA_MIXED_PSK:
            security_type_string = SECURITY_WPA2_WPA_MIXED_PSK;
            break;
        case CY_WCM_SECURITY_WPA_TKIP_ENT:
            security_type_string = SECURITY_WPA_TKIP_ENT;
            break;
        case CY_WCM_SECURITY_WPA_AES_ENT:
            security_type_string = SECURITY_WPA_AES_ENT;
            break;
        case CY_WCM_SECURITY_WPA_MIXED_ENT:
            security_type_string = SECURITY_WPA_MIXED_ENT;
            break;
        case CY_WCM_SECURITY_WPA2_TKIP_ENT:
            security_type_string = SECURITY_WPA2_TKIP_ENT;
            break;
        case CY_WCM_SECURITY_WPA2_AES_ENT:
            security_type_string = SECURITY_WPA2_AES_ENT;
            break;
        case CY_WCM_SECURITY_WPA2_MIXED_ENT:
            security_type_string = SECURITY_WPA2_MIXED_ENT;
            break;
        case CY_WCM_SECURITY_WPA2_FBT_ENT:
            security_type_string = SECURITY_WPA2_FBT_ENT;
            break;
        default:
            security_type_string = SECURITY_UNKNOWN;
            break;
    }

    printf(" %2" PRIu32 "   %-32s     %4d     %2d      %02X:%02X:%02X:%02X:%02X:%02X         %-15s\n",
        num_scan_result, result->SSID,
        result->signal_strength, result->channel, result->BSSID[0], result->BSSID[1],
        result->BSSID[2], result->BSSID[3], result->BSSID[4], result->BSSID[5],
        security_type_string);
}


/*******************************************************************************
 * Function Name: gpio_interrupt_handler
 *******************************************************************************
 * Summary:
 *  GPIO interrupt service routine. This function detects button presses and
 *  changes the type of the applied scan filter.
 *
 * Parameters:
 *  void *callback_arg : pointer to the variable passed to the ISR
 *  cyhal_gpio_event_t event : GPIO event type
 *
 * Return:
 *  None
 *
 *******************************************************************************/
void gpio_interrupt_handler(void *handler_arg, cyhal_gpio_event_t event) {
    /* Increment and check if the scan filter selected is invalid. If invalid,
     * reset to no scan filter.
     */
    if (++scan_filter_mode_select >= SCAN_FILTER_INVALID) {
        scan_filter_mode_select = SCAN_FILTER_NONE;
    }
}


/*******************************************************************************
* Function Name: error_handler
********************************************************************************
*
* Summary:
* This function processes unrecoverable errors such as any component
* initialization errors etc. In case of such error, the system will halt the
* CPU.
*
* Parameters:
* cy_rslt_t result: contains the result of an operation.
* char* message: contains the error message that is printed to the serial
* terminal.
*
* Note: If error occurs, interrupts are disabled.
*
*******************************************************************************/
void error_handler(cy_rslt_t result, char *message) {
    if (CY_RSLT_SUCCESS != result) {
        if (is_led_initialized) {
            cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_ON);
        }

        if (is_retarget_io_initialized && (NULL != message)) {
            ERR_INFO(("%s", message));
        }

        __disable_irq();
        CY_ASSERT(0);
    }
}


/* [] END OF FILE */
