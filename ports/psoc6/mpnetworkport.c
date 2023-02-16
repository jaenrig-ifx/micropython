#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "py/objlist.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "shared/netutils/netutils.h"
//#include "systick.h"
#include "pendsv.h"
#include "extmod/modnetwork.h"

#if MICROPY_PY_LWIP
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "lwip/dns.h"
#include "lwip/dhcp.h"
#include "lwip/apps/mdns.h"
#include "extmod/network_cyw43.h"
#include "drivers/cyw43/cyw43.h"

u32_t sys_now(void) {
    return mp_hal_ticks_ms();
}

#endif // MICROPY_PY_LWIP