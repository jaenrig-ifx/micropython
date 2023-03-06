/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Damien P. George
 * Copyright (c) 2023 Infineon Technologies AG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include "py/runtime.h"
#include "py/objstr.h"
#include "py/mphal.h"

#if MICROPY_PY_NETWORK_IFX_WHD

#include "lwip/netif.h"
#include "whd.h"
#include "whd_network_types.h"
#include "whd_wlioctl.h"
#include "extmod/network_ifx_whd.h"
#include "whd_wifi_api.h"
#include "modnetwork.h"

whd_driver_t whd;
whd_interface_t itf_sta;
whd_interface_t itf_ap;

typedef struct _network_ifx_whd_obj_t {
    mp_obj_base_t base;
    whd_driver_t *whd;
    whd_interface_t *itf;
    struct netif netif;
} network_ifx_whd_obj_t;

STATIC network_ifx_whd_obj_t network_ifx_whd_wl_sta = { { &mp_network_ifx_whd_type }, &whd, &itf_sta };
STATIC network_ifx_whd_obj_t network_ifx_whd_wl_ap = { { &mp_network_ifx_whd_type }, &whd, &itf_ap };

STATIC void network_ifx_whd_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
//     network_cyw43_obj_t *self = MP_OBJ_TO_PTR(self_in);
//     struct netif *netif = &self->cyw->netif[self->itf];
//     int status = cyw43_tcpip_link_status(self->cyw, self->itf);
//     const char *status_str;
//     if (status == CYW43_LINK_DOWN) {
//         status_str = "down";
//     } else if (status == CYW43_LINK_JOIN || status == CYW43_LINK_NOIP) {
//         status_str = "join";
//     } else if (status == CYW43_LINK_UP) {
//         status_str = "up";
//     } else if (status == CYW43_LINK_NONET) {
//         status_str = "nonet";
//     } else if (status == CYW43_LINK_BADAUTH) {
//         status_str = "badauth";
//     } else {
//         status_str = "fail";
//     }
//     mp_printf(print, "<CYW43 %s %s %u.%u.%u.%u>",
//         self->itf == CYW43_ITF_STA ? "STA" : "AP",
//         status_str,
//         netif->ip_addr.addr & 0xff,
//         netif->ip_addr.addr >> 8 & 0xff,
//         netif->ip_addr.addr >> 16 & 0xff,
//         netif->ip_addr.addr >> 24
//         );
}

STATIC mp_obj_t network_ifx_whd_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 0, 1, false);

    if (n_args == 0 || mp_obj_get_int(args[0]) == MOD_NETWORK_STA_IF) {
        return MP_OBJ_FROM_PTR(&network_ifx_whd_wl_sta);
    } else {
        return MP_OBJ_FROM_PTR(&network_ifx_whd_wl_ap);
    }
    return mp_const_none;
}

STATIC mp_obj_t network_ifx_whd_send_ethernet(mp_obj_t self_in, mp_obj_t buf_in) {
    network_ifx_whd_obj_t *self = MP_OBJ_TO_PTR(self_in);
    whd_interface_t itf = *(self->itf);
    mp_buffer_info_t buf;
    mp_get_buffer_raise(buf_in, &buf, MP_BUFFER_READ);
    whd_buffer_t *whd_buff = (whd_buffer_t *)&buf;
    int ret = whd_network_send_ethernet_data(itf, *whd_buff);
    if (ret) {
        mp_raise_OSError(-ret);
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(network_ifx_whd_send_ethernet_obj, network_ifx_whd_send_ethernet);

STATIC mp_obj_t network_ifx_whd_ioctl(mp_obj_t self_in, mp_obj_t cmd_in, mp_obj_t buf_in) {
//     network_cyw43_obj_t *self = MP_OBJ_TO_PTR(self_in);
//     mp_buffer_info_t buf;
//     mp_get_buffer_raise(buf_in, &buf, MP_BUFFER_READ | MP_BUFFER_WRITE);
//     cyw43_ioctl(self->cyw, mp_obj_get_int(cmd_in), buf.len, buf.buf, self->itf);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(network_ifx_whd_ioctl_obj, network_ifx_whd_ioctl);

/*******************************************************************************/
// network API

STATIC mp_obj_t network_ifx_whd_deinit(mp_obj_t self_in) {
//     network_ifx_whd_obj_t *self = MP_OBJ_TO_PTR(self_in);
//     cyw43_deinit(self->cyw);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(network_ifx_whd_deinit_obj, network_ifx_whd_deinit);

STATIC mp_obj_t network_ifx_whd_active(size_t n_args, const mp_obj_t *args) {
    network_ifx_whd_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    whd_interface_t itf = *(self->itf);

    if (n_args == 1) {
        // Is this function the most suitable? There is no whd public
        // function to get the link status
        return mp_obj_new_bool(whd_wifi_is_ready_to_transceive(itf));
    } else {
        if (mp_obj_is_true(args[1])) {
            whd_wifi_set_up(itf);
        } else {
            whd_wifi_set_down(itf);
        }
        return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(network_ifx_whd_active_obj, 1, 2, network_ifx_whd_active);

STATIC whd_scan_result_t whd_scan_result;
STATIC volatile whd_scan_status_t whd_scan_status;

STATIC void network_ifx_whd_scan_cb(whd_scan_result_t **result_ptr, void *user_data, whd_scan_status_t status) {
    mp_obj_t list = MP_OBJ_FROM_PTR(user_data);
    whd_scan_result_t *scan_result_ptr = *result_ptr;
    // Search for existing BSSID to remove duplicates
    bool found = false;
    size_t len;
    mp_obj_t *items;
    mp_obj_get_array(list, &len, &items);
    for (size_t i = 0; i < len; ++i) {
        mp_obj_tuple_t *t = MP_OBJ_TO_PTR(items[i]);
        if (memcmp(scan_result_ptr->BSSID.octet, ((mp_obj_str_t *)MP_OBJ_TO_PTR(t->items[1]))->data, sizeof(scan_result_ptr->BSSID.octet)) == 0) {
            if (scan_result_ptr->signal_strength > MP_OBJ_SMALL_INT_VALUE(t->items[3])) {
                t->items[3] = MP_OBJ_NEW_SMALL_INT(scan_result_ptr->signal_strength);
            }
            t->items[5] = MP_OBJ_NEW_SMALL_INT(MP_OBJ_SMALL_INT_VALUE(t->items[5]) + 1);
            found = true;
            break;
        }
    }

    // Add to list of results if wanted
    // Scan info tuple
    // 0 - ssid
    // 1 - bssid
    // 2 - channel
    // 3 - rssi
    // 4 - auth mode
    // 5 - Number of result appareances
    if (!found) {
        mp_obj_t tuple[6] = {
            mp_obj_new_bytes(scan_result_ptr->SSID.value, scan_result_ptr->SSID.length),
            mp_obj_new_bytes(scan_result_ptr->BSSID.octet, sizeof(scan_result_ptr->BSSID.octet)),
            MP_OBJ_NEW_SMALL_INT(scan_result_ptr->channel),
            MP_OBJ_NEW_SMALL_INT(scan_result_ptr->signal_strength),
            MP_OBJ_NEW_SMALL_INT(scan_result_ptr->security),
            MP_OBJ_NEW_SMALL_INT(1) // N
        };
        mp_obj_list_append(list, mp_obj_new_tuple(6, tuple));
    }

    // Update scan status
    whd_scan_status = status;
}

STATIC mp_obj_t network_ifx_whd_scan(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_passive, ARG_ssid, ARG_essid, ARG_bssid, ARG_channels };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_passive, MP_ARG_KW_ONLY | MP_ARG_BOOL, {.u_bool = false} },
        { MP_QSTR_ssid, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_essid, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_bssid, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_channels, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
    };

    network_ifx_whd_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    whd_interface_t itf = *(self->itf);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Deprecated kwarg
    if (args[ARG_essid].u_obj != mp_const_none) {
        args[ARG_ssid].u_obj = args[ARG_essid].u_obj;
    }

    // Extract scan type, if given.
    // Note: WHD driver provides also prohibited channel,
    // currently not added to keep the interface as similar
    // to the cy43w network module implementation.
    whd_scan_type_t scan_type = WHD_SCAN_TYPE_PASSIVE;
    if (args[ARG_passive].u_bool) {
        scan_type = WHD_SCAN_TYPE_ACTIVE;
    }

    // Extract ssid, if given
    whd_ssid_t *ssid_ptr = NULL;
    whd_ssid_t ssid;
    if (args[ARG_ssid].u_obj != mp_const_none) {
        ssid_ptr = &ssid;
        mp_buffer_info_t ssid_arg;
        mp_get_buffer_raise(args[ARG_ssid].u_obj, &ssid_arg, MP_BUFFER_READ);
        ssid.length = MIN(ssid_arg.len, SSID_NAME_SIZE);
        memcpy(ssid.value, ssid_arg.buf, ssid.length);
    }

    // Extract bssid, if given
    whd_mac_t *bssid_ptr = NULL;
    whd_mac_t bssid;
    if (args[ARG_bssid].u_obj != mp_const_none) {
        bssid_ptr = &bssid;
        mp_buffer_info_t bssid_arg;
        mp_get_buffer_raise(args[ARG_bssid].u_obj, &bssid_arg, MP_BUFFER_READ);
        memcpy(bssid.octet, bssid_arg.buf, 6);
    }

    // Extract channel list, if provided
    uint16_t channel_list[sizeof(uint8_t)] = {0};
    if (args[ARG_channels].u_obj != mp_const_none) {
        mp_buffer_info_t channels_arg;
        mp_get_buffer_raise(args[ARG_channels].u_obj, &channels_arg, MP_BUFFER_READ);
        memcpy(channel_list, channels_arg.buf, channels_arg.len);
        channel_list[channels_arg.len] = 0; // The channel list array needs to be zero terminated.
    }

    // Note: Optional extended parameters are not provided.
    whd_scan_status = WHD_SCAN_INCOMPLETE;
    mp_obj_t res = mp_obj_new_list(0, NULL);
    uint32_t ret = whd_wifi_scan(itf, scan_type, WHD_BSS_TYPE_ANY, ssid_ptr, bssid_ptr, channel_list, NULL, network_ifx_whd_scan_cb, &whd_scan_result, &res);
    if (ret != WHD_SUCCESS) {
        mp_raise_OSError(-ret);
    }

    // Wait for scan to finish, with a 10s timeout
    uint32_t start = mp_hal_ticks_ms();
    while (whd_scan_status && (mp_hal_ticks_ms() - start < 10000)) {
        MICROPY_EVENT_POLL_HOOK
    }

    return res;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(network_ifx_whd_scan_obj, 1, network_ifx_whd_scan);

STATIC mp_obj_t network_ifx_whd_connect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_ssid, ARG_key, ARG_auth, ARG_security, ARG_bssid, ARG_channel };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_ssid, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_key, MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_auth, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_security, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_bssid, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_channel, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    };

    network_ifx_whd_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    whd_interface_t itf = *(self->itf);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Deprecated kwarg
    if (args[ARG_auth].u_int != -1) {
        args[ARG_security] = args[ARG_auth];
    }

    // Extract the SSID.
    mp_buffer_info_t ssid;
    mp_get_buffer_raise(args[ARG_ssid].u_obj, &ssid, MP_BUFFER_READ);
    // TODO: Check length cannot be more than 32 bytes. Limited by whd lib. Validate argument!
    whd_ssid_t whd_ssid = {
        .length = ssid.len
    };
    memcpy(whd_ssid.value, ssid.buf, ssid.len);

    // Extract the key, if given.
    mp_buffer_info_t key;
    key.buf = NULL;
    if (args[ARG_key].u_obj != mp_const_none) {
        mp_get_buffer_raise(args[ARG_key].u_obj, &key, MP_BUFFER_READ);
    }

    // Extract the BSSID, if given.
    mp_buffer_info_t bssid;
    bssid.buf = NULL;
    if (args[ARG_bssid].u_obj != mp_const_none) {
        mp_get_buffer_raise(args[ARG_bssid].u_obj, &bssid, MP_BUFFER_READ);
        whd_mac_t whd_bssid;

        // Check if the given bssid matches the interface bssid
        uint32_t ret = whd_wifi_get_bssid(itf, &whd_bssid);
        if (ret != WHD_SUCCESS) {
            mp_raise_OSError(-ret);
        }

        if (bssid.len != 6) {
            if (strncmp(bssid.buf, (char *)whd_bssid.octet, 6)) {
                mp_raise_ValueError(NULL);
            }
        }
    }

    // Extract the security type, if given.
    whd_security_t auth_type;
    if (args[ARG_security].u_int == -1) {
        if (key.buf == NULL || key.len == 0) {
            auth_type = WHD_SECURITY_OPEN;
        } else {
            auth_type = WHD_SECURITY_WPA2_MIXED_PSK;
        }
    } else {
        auth_type = args[ARG_security].u_int;
    }

    // Extract channel, if given.
    if (args[ARG_channel].u_int == -1) {

        uint32_t ret = whd_wifi_set_channel(itf, args[ARG_channel].u_int);
        if (ret != WHD_SUCCESS) {
            mp_raise_OSError(-ret);
        }
    }

    // Start the WiFi join procedure.  It will run in the background.
    uint32_t ret = whd_wifi_join(itf, &whd_ssid, auth_type, key.buf, key.len);
    if (ret != WHD_SUCCESS) {
        mp_raise_OSError(-ret);
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(network_ifx_whd_connect_obj, 1, network_ifx_whd_connect);

STATIC mp_obj_t network_ifx_whd_disconnect(mp_obj_t self_in) {
    network_ifx_whd_obj_t *self = MP_OBJ_TO_PTR(self_in);
    whd_interface_t itf = *(self->itf);
    uint32_t ret = whd_wifi_leave(itf);
    if (ret != WHD_SUCCESS) {
        mp_raise_OSError(-ret);
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(network_ifx_whd_disconnect_obj, network_ifx_whd_disconnect);

STATIC mp_obj_t network_ifx_whd_isconnected(mp_obj_t self_in) {
    network_ifx_whd_obj_t *self = MP_OBJ_TO_PTR(self_in);
    whd_interface_t itf = *(self->itf);
    // Is this function the most suitable? There is no whd public
    // function to get the link status
    return mp_obj_new_bool(whd_wifi_is_ready_to_transceive(itf));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(network_ifx_whd_isconnected_obj, network_ifx_whd_isconnected);

STATIC mp_obj_t network_ifx_whd_ifconfig(size_t n_args, const mp_obj_t *args) {
    // network_ifx_whd_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    // Explore the integration with the network stack ... lwip
    // return mod_network_nic_ifconfig(&self->cyw->netif[self->itf], n_args - 1, args + 1);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(network_ifx_whd_ifconfig_obj, 1, 2, network_ifx_whd_ifconfig);

STATIC mp_obj_t network_ifx_whd_status(size_t n_args, const mp_obj_t *args) {
    network_ifx_whd_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    whd_interface_t itf = *(self->itf);

    if (n_args == 1) {
        // no arguments: return link status
        return MP_OBJ_NEW_SMALL_INT(whd_wifi_is_ready_to_transceive(itf));
    }

    // one argument: return status based on query parameter
    switch (mp_obj_str_get_qstr(args[1])) {
        case MP_QSTR_stations: {
            // return list of connected stations
            // if (self->itf != CYW43_ITF_AP) {
            //     mp_raise_ValueError(MP_ERROR_TEXT("AP required"));
            // }
            int num_stas;
            uint8_t macs[32 * 6];
            // cyw43_wifi_ap_get_stas(self->cyw, &num_stas, macs);
            mp_obj_t list = mp_obj_new_list(num_stas, NULL);
            for (int i = 0; i < num_stas; ++i) {
                mp_obj_t tuple[1] = {
                    mp_obj_new_bytes(&macs[i * 6], 6),
                };
                ((mp_obj_list_t *)MP_OBJ_TO_PTR(list))->items[i] = mp_obj_new_tuple(1, tuple);
            }
            return list;
        }
    }

    mp_raise_ValueError(MP_ERROR_TEXT("unknown status param"));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(network_ifx_whd_status_obj, 1, 2, network_ifx_whd_status);

static inline uint32_t nw_get_le32(const uint8_t *buf) {
    return buf[0] | buf[1] << 8 | buf[2] << 16 | buf[3] << 24;
}

static inline void nw_put_le32(uint8_t *buf, uint32_t x) {
    buf[0] = x;
    buf[1] = x >> 8;
    buf[2] = x >> 16;
    buf[3] = x >> 24;
}

STATIC mp_obj_t network_ifx_whd_config(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    network_ifx_whd_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    whd_interface_t itf = *(self->itf);

    if (kwargs->used == 0) {
        // Get config value
        if (n_args != 2) {
            mp_raise_TypeError(MP_ERROR_TEXT("must query one param"));
        }

        switch (mp_obj_str_get_qstr(args[1])) {
            case MP_QSTR_antenna: {
                uint8_t buf[4];
                uint32_t ret = whd_wifi_get_ioctl_buffer(itf, WLC_GET_ANTDIV, buf, 4);
                if (ret != WHD_SUCCESS) {
                    mp_raise_OSError(-ret);
                }
                return MP_OBJ_NEW_SMALL_INT(nw_get_le32(buf));
            }
            case MP_QSTR_channel: {
                uint32_t channel;
                uint32_t ret = whd_wifi_get_channel(itf, &channel);
                if (ret != WHD_SUCCESS) {
                    mp_raise_OSError(-ret);
                }
                return MP_OBJ_NEW_SMALL_INT(channel);
            }
            case MP_QSTR_ssid:
            case MP_QSTR_essid: {
                wl_bss_info_t bss_info;
                uint32_t ret = whd_wifi_get_bss_info(itf, &bss_info);
                if (ret != WHD_SUCCESS) {
                    mp_raise_OSError(-ret);
                }
                return mp_obj_new_str((const char *)bss_info.SSID, bss_info.SSID_len);
            }

            case MP_QSTR_security: {
                whd_security_t security;
                uint32_t ret = whd_wifi_get_ap_info(itf, NULL, &security);
                if (ret != WHD_SUCCESS) {
                    mp_raise_OSError(-ret);
                }
                return MP_OBJ_NEW_SMALL_INT(security);
            }

            case MP_QSTR_mac: {
                wl_bss_info_t bss_info;
                uint32_t ret = whd_wifi_get_bss_info(itf, &bss_info);
                if (ret != WHD_SUCCESS) {
                    mp_raise_OSError(-ret);
                }
                return mp_obj_new_bytes(bss_info.BSSID.octet, 6);
            }
            case MP_QSTR_txpower: {
                uint8_t buf[13];
                memcpy(buf, "qtxpower\x00\x00\x00\x00\x00", 13);
                uint32_t ret = whd_wifi_get_ioctl_buffer(itf, WLC_GET_VAR, buf, 13);
                if (ret != WHD_SUCCESS) {
                    mp_raise_OSError(-ret);
                }
                return MP_OBJ_NEW_SMALL_INT(nw_get_le32(buf) / 4);
            }

            case MP_QSTR_hostname: {
                const char *hostname = netif_get_hostname(&(self->netif));
                return mp_obj_new_str(hostname, strlen(hostname));
            }

            default:
                mp_raise_ValueError(MP_ERROR_TEXT("unknown config param"));
        }
    } else {
        // Set config value(s)
        if (n_args != 1) {
            mp_raise_TypeError(MP_ERROR_TEXT("can't specify pos and kw args"));
        }

        for (size_t i = 0; i < kwargs->alloc; ++i) {
            if (MP_MAP_SLOT_IS_FILLED(kwargs, i)) {
                mp_map_elem_t *e = &kwargs->table[i];
                switch (mp_obj_str_get_qstr(e->key)) {
                    case MP_QSTR_antenna: {
                        uint8_t buf[4];
                        nw_put_le32(buf, mp_obj_get_int(e->value));
                        // cyw43_ioctl(self->cyw, CYW43_IOCTL_SET_ANTDIV, 4, buf, self->itf);
                        break;
                    }
                    case MP_QSTR_channel: {
                        // cyw43_wifi_ap_set_channel(self->cyw, mp_obj_get_int(e->value));
                        break;
                    }
                    case MP_QSTR_ssid:
                    case MP_QSTR_essid: {
                        // size_t len;
                        // const char *str = mp_obj_str_get_data(e->value, &len);
                        // cyw43_wifi_ap_set_ssid(self->cyw, len, (const uint8_t *)str);
                        break;
                    }
                    case MP_QSTR_monitor: {
                        mp_int_t value = mp_obj_get_int(e->value);
                        uint8_t buf[9 + 4];
                        memcpy(buf, "allmulti\x00", 9);
                        nw_put_le32(buf + 9, value);
                        // cyw43_ioctl(self->cyw, CYW43_IOCTL_SET_VAR, 9 + 4, buf, self->itf);
                        nw_put_le32(buf, value);
                        // cyw43_ioctl(self->cyw, CYW43_IOCTL_SET_MONITOR, 4, buf, self->itf);
                        // if (value) {
                        //     self->cyw->trace_flags |= CYW43_TRACE_MAC;
                        // } else {
                        //     self->cyw->trace_flags &= ~CYW43_TRACE_MAC;
                        // }
                        // break;
                    }
                    case MP_QSTR_security: {
                        // cyw43_wifi_ap_set_auth(self->cyw, mp_obj_get_int(e->value));
                        break;
                    }
                    case MP_QSTR_key:
                    case MP_QSTR_password: {
                        // size_t len;
                        // const char *str = mp_obj_str_get_data(e->value, &len);
                        // cyw43_wifi_ap_set_password(self->cyw, len, (const uint8_t *)str);
                        break;
                    }
                    case MP_QSTR_pm: {
                        // cyw43_wifi_pm(self->cyw, mp_obj_get_int(e->value));
                        break;
                    }
                    case MP_QSTR_trace: {
                        // self->cyw->trace_flags = mp_obj_get_int(e->value);
                        break;
                    }
                    case MP_QSTR_txpower: {
                        mp_int_t dbm = mp_obj_get_int(e->value);
                        uint8_t buf[9 + 4];
                        memcpy(buf, "qtxpower\x00", 9);
                        nw_put_le32(buf + 9, dbm * 4);
                        // cyw43_ioctl(self->cyw, CYW43_IOCTL_SET_VAR, 9 + 4, buf, self->itf);
                        break;
                    }
                    #if !MICROPY_PY_NETWORK_CYW43_USE_LIB_DRIVER
                    case MP_QSTR_hostname: {
                        // const char *hostname = mp_obj_str_get_str(e->value);
                        // strncpy(self->cyw->hostname, hostname, MICROPY_BOARD_HOSTNAME_LENGTH);
                        // self->cyw->hostname[MICROPY_BOARD_HOSTNAME_LENGTH - 1] = 0;
                        break;
                    }
                    #endif
                    default:
                        mp_raise_ValueError(MP_ERROR_TEXT("unknown config param"));
                }
            }
        }

        return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(network_ifx_whd_config_obj, 1, network_ifx_whd_config);

/*******************************************************************************/
// class bindings

STATIC const mp_rom_map_elem_t network_ifx_whd_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_send_ethernet), MP_ROM_PTR(&network_ifx_whd_send_ethernet_obj) },
    { MP_ROM_QSTR(MP_QSTR_ioctl), MP_ROM_PTR(&network_ifx_whd_ioctl_obj) },

    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&network_ifx_whd_deinit_obj) }, // shall this be part of the module ??
    { MP_ROM_QSTR(MP_QSTR_active), MP_ROM_PTR(&network_ifx_whd_active_obj) },
    { MP_ROM_QSTR(MP_QSTR_scan), MP_ROM_PTR(&network_ifx_whd_scan_obj) },
    { MP_ROM_QSTR(MP_QSTR_connect), MP_ROM_PTR(&network_ifx_whd_connect_obj) },
    { MP_ROM_QSTR(MP_QSTR_disconnect), MP_ROM_PTR(&network_ifx_whd_disconnect_obj) },
    { MP_ROM_QSTR(MP_QSTR_isconnected), MP_ROM_PTR(&network_ifx_whd_isconnected_obj) },
    { MP_ROM_QSTR(MP_QSTR_ifconfig), MP_ROM_PTR(&network_ifx_whd_ifconfig_obj) },
    { MP_ROM_QSTR(MP_QSTR_status), MP_ROM_PTR(&network_ifx_whd_status_obj) },
    { MP_ROM_QSTR(MP_QSTR_config), MP_ROM_PTR(&network_ifx_whd_config_obj) },

    // Network WHD constants
    // Security modes
    { MP_ROM_QSTR(MP_QSTR_OPEN),                      MP_ROM_INT(WHD_SECURITY_OPEN) },
    { MP_ROM_QSTR(MP_QSTR_WEP_PSK),                   MP_ROM_INT(WHD_SECURITY_WEP_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WEP_SHARED),                MP_ROM_INT(WHD_SECURITY_WEP_SHARED) },
    { MP_ROM_QSTR(MP_QSTR_WPA_TKIP_PSK),              MP_ROM_INT(WHD_SECURITY_WPA_TKIP_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WPA_AES_PSK),               MP_ROM_INT(WHD_SECURITY_WPA_AES_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WPA_MIXED_PSK),             MP_ROM_INT(WHD_SECURITY_WPA_MIXED_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_AES_PSK),              MP_ROM_INT(WHD_SECURITY_WPA2_AES_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_TKIP_PSK),             MP_ROM_INT(WHD_SECURITY_WPA2_TKIP_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_MIXED_PSK),            MP_ROM_INT(WHD_SECURITY_WPA2_MIXED_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_FBT_PSK),              MP_ROM_INT(WHD_SECURITY_WPA2_FBT_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WPA3_SAE),                  MP_ROM_INT(WHD_SECURITY_WPA3_SAE) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_WPA_AES_PSK),          MP_ROM_INT(WHD_SECURITY_WPA2_WPA_AES_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_WPA_MIXED_PSK),       MP_ROM_INT(WHD_SECURITY_WPA2_WPA_MIXED_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WPA3_WPA2_PSK),             MP_ROM_INT(WHD_SECURITY_WPA3_WPA2_PSK) },
    { MP_ROM_QSTR(MP_QSTR_WPA_TKIP_ENT),              MP_ROM_INT(WHD_SECURITY_WPA_TKIP_ENT) },
    { MP_ROM_QSTR(MP_QSTR_WPA_MIXED_ENT),             MP_ROM_INT(WHD_SECURITY_WPA_MIXED_ENT) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_TKIP_ENT),             MP_ROM_INT(WHD_SECURITY_WPA2_TKIP_ENT) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_MIXED_ENT),            MP_ROM_INT(WHD_SECURITY_WPA2_MIXED_ENT) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_FBT_ENT),              MP_ROM_INT(WHD_SECURITY_WPA2_FBT_ENT) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_IBSS_OPEN),            MP_ROM_INT(WHD_SECURITY_IBSS_OPEN) },
    { MP_ROM_QSTR(MP_QSTR_WPA2_WPS_SECURE),           MP_ROM_INT(WHD_SECURITY_WPS_SECURE) },

    //
};
STATIC MP_DEFINE_CONST_DICT(network_ifx_whd_locals_dict, network_ifx_whd_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    mp_network_ifx_whd_type,
    MP_QSTR_IFX_WHD,
    MP_TYPE_FLAG_NONE,
    make_new, network_ifx_whd_make_new,
    print, network_ifx_whd_print,
    locals_dict, &network_ifx_whd_locals_dict
    );

#endif // MICROPY_PY_NETWORK_IFX_WHD
