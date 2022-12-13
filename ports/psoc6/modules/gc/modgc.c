// std includes
#include <stdio.h>


// micropython includes
#include "py/gc.h"


#if MICROPY_ENABLE_GC

void gc_collect(void) {
    printf("gc_collect\n");

    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
//    void *dummy;
    gc_collect_start();
//    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
}

#endif
