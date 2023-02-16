// std includes
#include "stdio.h"


// micropython includes
#include "mpconfigport.h"
#include "mphalport.h"
#include "shared/timeutils/timeutils.h"


// MTB includes
#include "cyhal.h"
#include "cy_retarget_io.h"


// port-specific includes
#include "drivers/machine/psoc6_gpio.h"
#include "modules/machine/pins.h"

#if MICROPY_PY_NETWORK_CYW43
#include "lib/cyw43-driver/src/cyw43.h"
#endif

extern cyhal_rtc_t psoc6_rtc;
extern cyhal_timer_t psoc6_timer;


void mp_hal_delay_ms(mp_uint_t ms) {
    cyhal_system_delay_ms(ms);
}


void mp_hal_delay_us(mp_uint_t us) {
    cyhal_system_delay_us(us);
}


uint64_t mp_hal_time_ns(void) {
    struct tm current_date_time = {0};
    cy_rslt_t result = cyhal_rtc_read(&psoc6_rtc, &current_date_time);

    if (CY_RSLT_SUCCESS != result) {
        mp_raise_ValueError(MP_ERROR_TEXT("cyhal_rtc_read failed !"));
    }

    uint64_t s = timeutils_seconds_since_epoch(current_date_time.tm_year, current_date_time.tm_mon, current_date_time.tm_mday,
        current_date_time.tm_hour, current_date_time.tm_min, current_date_time.tm_sec);
    return s * 1000000000ULL;
}


mp_uint_t mp_hal_ticks_ms(void) {
    return cyhal_timer_read(&psoc6_timer) / 1000;
}


mp_uint_t mp_hal_ticks_us(void) {
    return cyhal_timer_read(&psoc6_timer);
}


mp_uint_t mp_hal_ticks_cpu(void) {
    return cyhal_timer_read(&psoc6_timer);
}


uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags) {
    mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_stdio_poll not implemented !"));
    uintptr_t ret = 0;
    return ret;
}


int mp_hal_stdin_rx_chr(void) {
    uint8_t c = 0;

    cyhal_uart_getc(&cy_retarget_io_uart_obj, &c, 0);
    return c;
}


void mp_hal_pin_od_low(mp_hal_pin_obj_t pin) {
    gpio_clear_value(pin);
}


void mp_hal_pin_od_high(mp_hal_pin_obj_t pin) {
    gpio_set_value(pin);
}


int mp_hal_pin_read(mp_hal_pin_obj_t pin) {
    return gpio_get_value(pin);
}


void mp_hal_pin_open_drain(mp_hal_pin_obj_t pin) {
    cyhal_gpio_configure(pin, CYHAL_GPIO_DIR_INPUT,  CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW);
}


uint8_t mp_hal_pin_name(mp_hal_pin_obj_t pin) {
    return pin;
}


mp_hal_pin_obj_t mp_hal_get_pin_obj(mp_obj_t obj) {
    return pin_addr_by_name(obj);
}

// Support for Network classes
// Generate a random locally administered MAC address (LAA)
void mp_hal_generate_laa_mac(int idx, uint8_t buf[6]) {
    /*uint8_t *id = (uint8_t *)MP_HAL_UNIQUE_ID_ADDRESS;
    buf[0] = 0x02; // LAA range
    buf[1] = (id[11] << 4) | (id[10] & 0xf);
    buf[2] = (id[9] << 4) | (id[8] & 0xf);
    buf[3] = (id[7] << 4) | (id[6] & 0xf);
    buf[4] = id[2];
    buf[5] = (id[0] << 2) | idx;*/
}

// A board can override this if needed
MP_WEAK void mp_hal_get_mac(int idx, uint8_t buf[6]) {
    #if MICROPY_PY_NETWORK_CYW43
    // The mac should come from cyw43 otp when CYW43_USE_OTP_MAC is defined
    // This is loaded into the state after the driver is initialised
    // cyw43_hal_generate_laa_mac is only called by the driver to generate a mac if otp is not set
    memcpy(buf, cyw43_state.mac, 6);
    #else
    mp_hal_generate_laa_mac(idx, buf);
    #endif
}

void mp_hal_get_mac_ascii(int idx, size_t chr_off, size_t chr_len, char *dest) {
    static const char hexchr[16] = "0123456789ABCDEF";
    uint8_t mac[6];
    mp_hal_get_mac(idx, mac);
    for (; chr_len; ++chr_off, --chr_len) {
        *dest++ = hexchr[mac[chr_off >> 1] >> (4 * (1 - (chr_off & 1))) & 0xf];
    }
}

// TODO: move to another file or define as macro in mpconfigport.h
mp_uint_t begin_atomic_section() {
    __disable_irq();
    return 0;
}


void end_atomic_section(mp_uint_t state) {
    __enable_irq();
}
