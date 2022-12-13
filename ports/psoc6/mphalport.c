#include "stdio.h"


#include "mpconfigport.h"
#include "mphalport.h"


#include "shared/timeutils/timeutils.h"


#include "cyhal.h"
#include "cy_retarget_io.h"


extern cyhal_rtc_t psoc6_rtc;


void mp_hal_delay_ms(mp_uint_t ms) {
    printf("mp_hal_delay_ms not implemented !\n");
    // absolute_time_t t = make_timeout_time_ms(ms);
    // while (!time_reached(t)) {
    //     MICROPY_EVENT_POLL_HOOK_FAST;
    //     best_effort_wfe_or_timeout(t);
    // }
}

uint64_t mp_hal_time_ns(void) {
    // datetime_t t;
    // rtc_get_datetime(&t);
    struct tm current_date_time = {0};
    cy_rslt_t result = cyhal_rtc_read(&psoc6_rtc, &current_date_time);

    if (CY_RSLT_SUCCESS != result) {
    printf("cyhal_rtc_read failed !\n");
    }

    uint64_t s = timeutils_seconds_since_epoch(current_date_time.tm_year, current_date_time.tm_mon, current_date_time.tm_mday,
                                               current_date_time.tm_hour, current_date_time.tm_min, current_date_time.tm_sec);
    return s * 1000000000ULL;
}


void mp_hal_delay_us(mp_uint_t us) {
    printf("mp_hal_delay_us not implemented !\n");
//    sleep_us(us);
}


mp_uint_t mp_hal_ticks_us(void) {
//    printf("mp_hal_ticks_us not implemented !\n");
  return( 0UL );
//    return time_us_32();
}


mp_uint_t mp_hal_ticks_ms(void) {
//    printf("mp_hal_ticks_ms not implemented !\n");
  return( 0UL );
}


mp_uint_t begin_atomic_section() {
    __disable_irq();
    return( 0 );
}
// // Entering a critical section.
// extern uint32_t mp_thread_begin_atomic_section(void);
// extern void mp_thread_end_atomic_section(uint32_t);
// #define MICROPY_BEGIN_ATOMIC_SECTION()     mp_thread_begin_atomic_section()
// #define MICROPY_END_ATOMIC_SECTION(state)  mp_thread_end_atomic_section(state)


void end_atomic_section(mp_uint_t state) {
    __enable_irq();
}


uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags) {
    printf("mp_hal_stdio_poll not implemented !");
    uintptr_t ret = 0;
    // #if MICROPY_HW_ENABLE_USBDEV
    // poll_cdc_interfaces();
    // #endif
    // #if MICROPY_HW_ENABLE_UART_REPL || MICROPY_HW_ENABLE_USBDEV
    // if ((poll_flags & MP_STREAM_POLL_RD) && ringbuf_peek(&stdin_ringbuf) != -1) {
    //     ret |= MP_STREAM_POLL_RD;
    // }
    // #endif
    // #if MICROPY_PY_OS_DUPTERM
    // ret |= mp_uos_dupterm_poll(poll_flags);
    // #endif
    return ret;
}


int mp_hal_stdin_rx_chr(void) {
    uint8_t c = 0;

    cyhal_uart_getc(&cy_retarget_io_uart_obj, &c, 0);
    return c;
}
