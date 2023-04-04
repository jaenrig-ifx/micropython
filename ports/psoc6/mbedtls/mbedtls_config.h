#ifndef PSOC6_MPY_MBEDTLS_USER_CONFIG_HEADER
#define PSOC6_MPY_MBEDTLS_USER_CONFIG_HEADER

// Used to add MPY settings to MTB settings in file "boards/mtb_shared/wifi-core-freertos-lwip-mbedtls/release-v1.0.0/configs/mbedtls_user_config.h"
// such that both use an identical set of options.
// Further MTB configs can be found at : boards/mtb_shared/mbedtls/mbedtls-2.25.0/configs/

// Set mbedtls configuration
#define MBEDTLS_ECP_NIST_OPTIM
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED

// // // Enable mbedtls modules
#define MBEDTLS_GCM_C
#define MBEDTLS_HAVE_TIME
#define MBEDTLS_HAVE_TIME_DATE

// // #define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_CIPHER_MODE_CTR
#define MBEDTLS_CIPHER_MODE_CBC

// Time hook
// #include <time.h>
// time_t psoc6_rtctime_seconds(time_t *timer);
// #define MBEDTLS_PLATFORM_TIME_MACRO psoc6_rtctime_seconds

// Set MicroPython-specific options.
#define MICROPY_MBEDTLS_CONFIG_BARE_METAL (1)

// Include common mbedtls configuration.
#ifdef MICROPY_CONFIG_ROM_LEVEL
#include "extmod/mbedtls/mbedtls_config_common.h"
#endif

// #undef MBEDTLS_ENTROPY_HARDWARE_ALT

#endif /* PSOC6_MPY_MBEDTLS_USER_CONFIG_HEADER */
