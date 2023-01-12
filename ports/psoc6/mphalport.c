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


extern cyhal_rtc_t psoc6_rtc;


void mp_hal_delay_ms(mp_uint_t ms) {
    // mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_delay_ms not implemented !\n"));
    cyhal_system_delay_ms(ms);
}


void mp_hal_delay_us(mp_uint_t us) {
    // mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_delay_us not implemented !"));
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
//     mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_ticks_ms not implemented !"));
    return 0UL;
}


mp_uint_t mp_hal_ticks_us(void) {
//     mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_ticks_us not implemented !"));
    return 0UL;
//    return time_us_32();
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



void mp_hal_delay_us_fast(mp_uint_t us) {
    // mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_delay_us_fast not implemented !"));
    mp_hal_delay_us(us);
}

void mp_hal_pin_od_low(mp_hal_pin_obj_t pin) {
    gpio_clear_value(pin);
    // mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_pin_od_low not implemented !"));
}

void mp_hal_pin_od_high(mp_hal_pin_obj_t pin) {
    gpio_set_value(pin);
    // mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_pin_od_high not implemented !"));
}

int mp_hal_pin_read(mp_hal_pin_obj_t pin) {
    return gpio_get_value(pin);

    // mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_pin_read not implemented !"));
}

void mp_hal_pin_open_drain(mp_hal_pin_obj_t pin) {
//    mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_open_drain not implemented !"));
    printf("mp_hal_pin_open_drain for pin %d\n", pin);
    gpio_init(pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW, true);
}

uint8_t mp_hal_pin_name(mp_hal_pin_obj_t pin) {
// char *mp_hal_pin_name(mp_hal_pin_obj_t pin) {

    // static char buf[20];
    // memset(buf, 0, sizeof(buf));
    // sprintf(buf, "%d", pin);
    return pin;
    // mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_pin_name not implemented !"));
}




mp_hal_pin_obj_t mp_hal_get_pin_obj(mp_obj_t obj) {
    int pin = pin_find(obj, (const machine_pin_obj_t *)machine_pin_obj, MP_ARRAY_SIZE(machine_pin_obj));
    return pin;
}
