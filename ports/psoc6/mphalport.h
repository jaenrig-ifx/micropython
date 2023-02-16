// std includes
#include <stdio.h>
#include <unistd.h>


// micropython includes
#include "py/mpconfig.h"
#include "py/runtime.h"


// MTB includes
#include "cyhal.h"

// port-specific includes
#include "irq.h"

#define MP_HAL_PIN_PULL_NONE            (0)
#define MP_HAL_PIN_MODE_OUTPUT          (1)
#define MP_HAL_PIN_MODE_INPUT           (2)
//! Move to mcuconfig file
#define NVIC_PRIORITYGROUP_4            ((uint32_t)0x00000003)

#define MP_HAL_PIN_FMT   "%u"
#define mp_hal_pin_obj_t uint

#define mp_hal_delay_us_fast mp_hal_delay_us
#define mp_hal_pin_low(p)   mp_hal_pin_od_low(p)

#define mp_hal_pin_high(p) mp_hal_pin_od_high(p)

extern __attribute__((weak)) int _write(int fd, const char *ptr, int len);


void mp_hal_delay_us(mp_uint_t us);
void mp_hal_delay_us_fast(mp_uint_t us);
void mp_hal_delay_ms(mp_uint_t ms);

uint64_t mp_hal_time_ns(void);

mp_uint_t mp_hal_ticks_us(void);
mp_uint_t mp_hal_ticks_ms(void);
mp_uint_t mp_hal_ticks_cpu(void);

//! Adding to support networking
void mp_hal_pin_config(mp_hal_pin_obj_t pin, uint32_t mode, uint32_t pull, uint32_t alt);
void mp_hal_pin_od_low(mp_hal_pin_obj_t pin);
void mp_hal_pin_od_high(mp_hal_pin_obj_t pin);
void mp_hal_pin_open_drain(mp_hal_pin_obj_t pin);


int mp_hal_pin_read(mp_hal_pin_obj_t pin);
uint8_t mp_hal_pin_name(mp_hal_pin_obj_t pin);
mp_hal_pin_obj_t mp_hal_get_pin_obj(mp_obj_t obj);


uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags);

int mp_hal_stdin_rx_chr(void);


// Send string of given length
STATIC inline void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    int r = write(STDOUT_FILENO, str, len);
    (void)r;
}

enum {
    MP_HAL_MAC_WLAN0 = 0
};

void mp_hal_generate_laa_mac(int idx, uint8_t buf[6]);
void mp_hal_get_mac(int idx, uint8_t buf[6]);
void mp_hal_get_mac_ascii(int idx, size_t chr_off, size_t chr_len, char *dest);

mp_uint_t begin_atomic_section();
void end_atomic_section(mp_uint_t state);
