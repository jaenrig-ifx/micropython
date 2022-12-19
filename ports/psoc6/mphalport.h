// std includes
#include <stdio.h>
#include <unistd.h>


// micropython includes
#include "py/mpconfig.h"
#include "py/runtime.h"


// MTB includes
#include "cyhal.h"


extern __attribute__((weak)) int _write(int fd, const char *ptr, int len);


#define RAISE_ERRNO(err_flag, error_val) \
    { if (err_flag == -1) \
      { mp_raise_OSError(error_val); } }



/*
Note: Generally, the functions in the mp_* files are defined as static functions by the community,
probably to keep their scope limited to the file and prevent miscompilation for other ports. The below
function is an example of that. However, since it is later defined in a mphalport.c file, it cannot be
a static function. Hence it was later declared as a simple (non-static) function. The question would be
whether to keep a mphalport.c file in the end (if these function implementations are pretty small). */


// mp_uint_t begin_atomic_section();
// void end_atomic_section(mp_uint_t state);



void mp_hal_delay_us(mp_uint_t us);

mp_uint_t mp_hal_ticks_ms(void);
mp_uint_t mp_hal_ticks_us(void);


static inline mp_uint_t mp_hal_ticks_cpu(void) {
    // ticks_cpu() is defined as using the highest-resolution timing source
    // in the system. This is usually a CPU clock, but doesn't have to be.
    // return time_us_32();

    // printf("mp_hal_ticks_cpu not implemented !");
    mp_raise_NotImplementedError(MP_ERROR_TEXT("mp_hal_ticks_cpu not implemented !"));
    return 0;
}


/*
 * Core UART functions to implement for a port
 */
// Receive single character
int mp_hal_stdin_rx_chr(void);


// Send string of given length
STATIC inline void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    int r = write(STDOUT_FILENO, str, len);
    (void)r;

    // // Either convert str to \0 terminated string in order to be able to use printf
    // char buf[len + 1];
    // memcpy(buf, str, len);
    // buf[len] = 0;
    // printf(buf);

    // or use underlying method directly => this is compiler dependent !
    // _write(STDOUT_FILENO, str, len);
}
