// std includes
#include <stdio.h>
#include <unistd.h>


// micropython includes
#include "py/mpconfig.h"
#include "py/runtime.h"


// MTB includes
#include "cyhal.h"


// port-specific includes


#define MP_HAL_PIN_FMT   "%u"
#define mp_hal_pin_obj_t uint


extern __attribute__((weak)) int _write(int fd, const char *ptr, int len);


void mp_hal_delay_ms(mp_uint_t ms);
void mp_hal_delay_us(mp_uint_t us);
void mp_hal_delay_us_fast(mp_uint_t us);

uint64_t mp_hal_time_ns(void);


mp_uint_t mp_hal_ticks_ms(void);
mp_uint_t mp_hal_ticks_us(void);


void mp_hal_pin_od_low(mp_hal_pin_obj_t pin);
void mp_hal_pin_od_high(mp_hal_pin_obj_t pin);
void mp_hal_pin_open_drain(mp_hal_pin_obj_t pin);


int mp_hal_pin_read(mp_hal_pin_obj_t pin);
uint8_t mp_hal_pin_name(mp_hal_pin_obj_t pin);
// char *mp_hal_pin_name(mp_hal_pin_obj_t pin);

mp_hal_pin_obj_t mp_hal_get_pin_obj(mp_obj_t obj);


static inline mp_uint_t mp_hal_ticks_cpu(void) {
    mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_ticks_cpu not implemented !"));
    return 0;
}


uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags);

int mp_hal_stdin_rx_chr(void);


// Send string of given length
STATIC inline void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    int r = write(STDOUT_FILENO, str, len);
    (void)r;
}



mp_uint_t begin_atomic_section();
void end_atomic_section(mp_uint_t state);
