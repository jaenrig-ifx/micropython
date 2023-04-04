
// Including file from MTB assets to make sure they match. 
// Workaround : in that file LWIP_IPV6 must be set to zero due to an MPY bug. This is achieved by a perl patch
// in the Makefile, target "all". To be improved.
#include "boards/mtb_shared/wifi-core-freertos-lwip-mbedtls/release-v1.0.0/configs/lwipopts.h"

// changed by JB because of modlwip issue if both IP4 and IP6 are enabled !
// #undef  LWIP_IPV6
// #define LWIP_IPV6                       (0)

typedef uint32_t sys_prot_t;
