#include <py/mpconfig.h>

#ifdef MICROPY_SSL_MBEDTLS

#include "mbedtls_config.h"

// #include "hardware/rtc.h"
#include "shared/timeutils/timeutils.h"

// extern uint8_t rosc_random_u8(size_t cycles);

// int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen) {
//     *olen = len;
//     for (size_t i = 0; i < len; i++) {
//         output[i] = rosc_random_u8(8);
//     }
//     return 0;
// }

time_t psoc6_rtctime_seconds(time_t *timer) {
    datetime_t t;
    rtc_get_datetime(&t);
    return timeutils_seconds_since_epoch(t.year, t.month, t.day, t.hour, t.min, t.sec);
}

#endif
