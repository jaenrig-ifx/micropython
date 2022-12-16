// std includes
#include <stdio.h>


// micropython includes
#include "py/gc.h"


#if MICROPY_ENABLE_GC

void gc_collect(void) {
    gc_collect_start();
    gc_collect_end();
    gc_dump_info();
}

#endif
