
// std includes
#include <stdio.h>
#include <unistd.h>



// micropython includes
#include "py/mpconfig.h"


// MTB includes
#include "cyhal.h"
#include "cy_retarget_io.h"


extern __attribute__((weak)) int _write(int fd, const char *ptr, int len);


/*
 * Core UART functions to implement for a port
 */
// Receive single character
int mp_hal_stdin_rx_chr(void) {
    uint8_t c = 0;

    cyhal_uart_getc(&cy_retarget_io_uart_obj, &c, 0);
    return c;
}


// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
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
