// std includes
#include "stdio.h"

// micropython includes
#include "mpconfigport.h"
#include "mphalport.h"
#include "shared/timeutils/timeutils.h"

// MTB includes
#include "cyhal.h"
#include "cy_retarget_io.h"

extern cyhal_rtc_t   psoc6_rtc;
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
    uint64_t val = cyhal_timer_read(&psoc6_timer);
    //mp_printf(&mp_plat_print, "\nXTick val: %d", val);
    //mp_printf("Tick value: %d\n", val);

    return val/10; //cyhal_timer_read(&psoc6_timer);
}

mp_uint_t mp_hal_ticks_us(void) {
    return cyhal_timer_read(&psoc6_timer)*1000;
}

void mp_hal_delay_us_fast(mp_uint_t us){
    mp_hal_ticks_us();
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


// TODO: move to another file or define as macro in mpconfigport.h
mp_uint_t begin_atomic_section() {
    __disable_irq();
    return 0;
}


void end_atomic_section(mp_uint_t state) {
    __enable_irq();
}
