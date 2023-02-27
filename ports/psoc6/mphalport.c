// std includes
#include "stdbool.h"   // because of missing include in shared/timeutils/timeutils.h
#include "stdio.h"


// micropython includes
#include "mpconfigport.h"
// #include "mphalport.h"

#include "shared/timeutils/timeutils.h"

#if MICROPY_PY_NETWORK_CYW43
#include "lib/cyw43-driver/src/cyw43.h"
#endif

// MTB includes
#include "cyhal.h"
#include "cy_retarget_io.h"


// port-specific includes
#include "drivers/machine/psoc6_gpio.h"
#include "modules/machine/pins.h"


extern cyhal_rtc_t psoc6_rtc;
extern cyhal_timer_t psoc6_timer;
extern cyw43_t cyw43_state;

void mp_hal_delay_ms(mp_uint_t ms) {
    cyhal_system_delay_ms(ms);
}


void mp_hal_delay_us(mp_uint_t us) {
    cyhal_system_delay_us(us);
}


// issues may arise if time is incremented only each second, but also if ticks are used instead of time.
uint64_t mp_hal_time_ns(void) {
    // struct tm current_date_time = {0};
    // cy_rslt_t result = cyhal_rtc_read(&psoc6_rtc, &current_date_time);

    // if (CY_RSLT_SUCCESS != result) {
    //     mp_raise_ValueError(MP_ERROR_TEXT("cyhal_rtc_read failed !"));
    // }

    // uint64_t s = timeutils_seconds_since_epoch(current_date_time.tm_year, current_date_time.tm_mon, current_date_time.tm_mday,
    //     current_date_time.tm_hour, current_date_time.tm_min, current_date_time.tm_sec);
    // return s * 1000000000ULL;

    return cyhal_timer_read(&psoc6_timer) * 1000;
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
    printf("mp_hal_stdio_poll\n");
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


// TODO: move to another file or define as macro in mpconfigport.h
mp_uint_t begin_atomic_section() {
    // __disable_irq();
    // return 0;
    return cyhal_system_critical_section_enter();
}


void end_atomic_section(mp_uint_t state) {
    // __enable_irq();
    cyhal_system_critical_section_exit(state);
}









// Generate a random locally administered MAC address (LAA)
void mp_hal_generate_laa_mac(int idx, uint8_t buf[6]) {
    printf("mp_hal_generate_laa_mac\n");
    // #ifndef NDEBUG
    // printf("Warning: No MAC in OTP, generating MAC from board id\n");
    // #endif
    // pico_unique_board_id_t pid;
    // pico_get_unique_board_id(&pid);
    // buf[0] = 0x02; // LAA range
    // buf[1] = (pid.id[7] << 4) | (pid.id[6] & 0xf);
    // buf[2] = (pid.id[5] << 4) | (pid.id[4] & 0xf);
    // buf[3] = (pid.id[3] << 4) | (pid.id[2] & 0xf);
    // buf[4] = pid.id[1];
    // buf[5] = (pid.id[0] << 2) | idx;
}

// A board can override this if needed
MP_WEAK void mp_hal_get_mac(int idx, uint8_t buf[6]) {
    printf("mp_hal_get_mac\n");
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
    printf("mp_hal_get_mac_ascii\n");
    static const char hexchr[16] = "0123456789ABCDEF";
    uint8_t mac[6];
    mp_hal_get_mac(idx, mac);
    for (; chr_len; ++chr_off, --chr_len) {
        *dest++ = hexchr[mac[chr_off >> 1] >> (4 * (1 - (chr_off & 1))) & 0xf];
    }
}



// Shouldn't be used, needed by cyw43-driver in debug build.
uint32_t storage_read_blocks(uint8_t *dest, uint32_t block_num, uint32_t num_blocks) {
    printf("storage_read_blocks\n");
    // panic_unsupported();
    return 0UL;
}


// const char fw_4343WA1_7_45_98_50_start[426094];
