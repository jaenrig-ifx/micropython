// std includes
#include <stdio.h>
#include <unistd.h>


// micropython includes
#include "py/mpconfig.h"
#include "py/runtime.h"


// MTB includes
#include "cyhal.h"

extern __attribute__((weak)) int _write(int fd, const char *ptr, int len);

uint64_t mp_hal_time_ns(void);
mp_uint_t mp_hal_ticks_us(void);
mp_uint_t mp_hal_ticks_ms(void);
mp_uint_t mp_hal_ticks_cpu(void);
void mp_hal_delay_us(mp_uint_t us);
void mp_hal_delay_ms(mp_uint_t ms);


uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags);

int mp_hal_stdin_rx_chr(void);


// Send string of given length
STATIC inline void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    int r = write(STDOUT_FILENO, str, len);
    (void)r;
}

mp_uint_t begin_atomic_section();
void end_atomic_section(mp_uint_t state);
