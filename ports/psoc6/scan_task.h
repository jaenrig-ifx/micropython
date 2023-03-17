/*******************************************************************************
* File Name: scan_task.h
*
* Description: This file includes the macros, enumerations, and function
* prototypes used in scan_task.c
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
 *  Include guard
 ******************************************************************************/
#ifndef SOURCE_SCAN_TASK_H_
#define SOURCE_SCAN_TASK_H_


/*******************************************************************************
 * Header file includes
 ******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "cy_wcm.h"
#include <stdio.h>


// /*******************************************************************************
//  * Macros
//  ******************************************************************************/

// /* Provide the value of SSID which should be used to filter the scan results.*/
// #define SCAN_FOR_SSID_VALUE                     "SSID"

// /* Provide the value of the MAC address which should be used to filter the scan
//  * results. For example, MAC Address: 12:34:56:78:9A:BC should be entered as
//  * shown below.
//  */
// #define SCAN_FOR_MAC_ADDRESS                     0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC

// /* Provide the value of the ISM band (2.4 GHz/ 5 GHz/ both) which should be used
//  * to filter the scan results. The valid values are provided in the enumeration
//  * cy_wcm_wifi_band_t in cy_wcm.h.
//  * Note: CY8CPROTO-062-4343W is a single band device and can only scan for
//  * networks in 2.4 GHz.
//  */
// #define SCAN_FOR_BAND_VALUE                     CY_WCM_WIFI_BAND_ANY

// /* Provide the value of the RSSI range that should be used to filter the scan
//  * results. The valid values are provided in the enumeration
//  * cy_wcm_scan_rssi_range_t in cy_wcm.h.
//  */
// #define SCAN_FOR_RSSI_VALUE                     CY_WCM_SCAN_RSSI_EXCELLENT

// /* The delay in milliseconds between successive scans.*/
// #define SCAN_DELAY_MS                           (3000u)




// #define MAX_SECURITY_STRING_LENGTH              (15)

// #define SECURITY_OPEN                           "OPEN"
// #define SECURITY_WEP_PSK                        "WEP-PSK"
// #define SECURITY_WEP_SHARED                     "WEP-SHARED"
// #define SECURITY_WEP_TKIP_PSK                   "WEP-TKIP-PSK"
// #define SECURITY_WPA_TKIP_PSK                   "WPA-TKIP-PSK"
// #define SECURITY_WPA_AES_PSK                    "WPA-AES-PSK"
// #define SECURITY_WPA_MIXED_PSK                  "WPA-MIXED-PSK"
// #define SECURITY_WPA2_AES_PSK                   "WPA2-AES-PSK"
// #define SECURITY_WPA2_TKIP_PSK                  "WPA2-TKIP-PSK"
// #define SECURITY_WPA2_MIXED_PSK                 "WPA2-MIXED-PSK"
// #define SECURITY_WPA2_FBT_PSK                   "WPA2-FBT-PSK"
// #define SECURITY_WPA3_SAE                       "WPA3-SAE"
// #define SECURITY_WPA2_WPA_AES_PSK               "WPA2-WPA-AES-PSK"
// #define SECURITY_WPA2_WPA_MIXED_PSK             "WPA2-WPA-MIXED-PSK"
// #define SECURITY_WPA3_WPA2_PSK                  "WPA3-WPA2-PSK"
// #define SECURITY_WPA_TKIP_ENT                   "WPA-TKIP-ENT"
// #define SECURITY_WPA_AES_ENT                    "WPA-AES-ENT"
// #define SECURITY_WPA_MIXED_ENT                  "WPA-MIXED-ENT"
// #define SECURITY_WPA2_TKIP_ENT                  "WPA2-TKIP-ENT"
// #define SECURITY_WPA2_AES_ENT                   "WPA2-AES-ENT"
// #define SECURITY_WPA2_MIXED_ENT                 "WPA2-MIXED-ENT"
// #define SECURITY_WPA2_FBT_ENT                   "WPA2-FBT-ENT"
// #define SECURITY_IBSS_OPEN                      "IBSS-OPEN"
// #define SECURITY_WPS_SECURE                     "WPS-SECURE"
// #define SECURITY_UNKNOWN                        "UNKNOWN"

#define PRINT_SCAN_TEMPLATE()                   printf("\n----------------------------------------------------------------------------------------------------\n" \
    "  #                  SSID                  RSSI   Channel       MAC Address              Security\n" \
    "----------------------------------------------------------------------------------------------------\n");

// #define APP_INFO(x)           do { printf("\nInfo: "); printf x;} while (0);
// #define ERR_INFO(x)           do { printf("\nError: "); printf x;} while (0);


/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/

// void gpio_interrupt_handler(void *handler_arg, cyhal_gpio_event_t event);
// void error_handler(cy_rslt_t result, char *message);


/*******************************************************************************
 * Enumerations
 ******************************************************************************/
/* This enumeration contains the different types of scan filters available. They
 * are,
 * SCAN_FILTER_NONE: No scan filter.
 * SCAN_FILTER_SSID: The scan results are filtered by SSID.
 * SCAN_FILTER_MAC: The scan results are filtered by MAC address of the AP.
 * SCAN_FILTER_BAND: The scan results are filtered by the ISM band (2.4 or 5 GHz
 * or both) that the AP is occupying.
 * SCAN_FILTER_RSSI: The scan results are filtered by the RSSI strength.
 * SCAN_FILTER_INVALID: Invalid scan filter.
 */



/*******************************************************************************
 * Global Variables
 ******************************************************************************/
/* Network connection task handle */
// extern TaskHandle_t scan_task_handle;
// extern bool is_retarget_io_initialized;
// extern bool is_led_initialized;

#endif /* SOURCE_SCAN_TASK_H_ */


/* [] END OF FILE */
