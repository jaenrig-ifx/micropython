/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>

#include "py/mperrno.h"
#include "py/mphal.h"
#include "sdio.h"

#if MICROPY_PY_NETWORK_CYW43

// If no custom SDIO pins defined, use the default ones
/*#ifndef MICROPY_HW_SDIO_CK
#define MICROPY_HW_SDIO_D0      (pin_C8)
#define MICROPY_HW_SDIO_D1      (pin_C9)
#define MICROPY_HW_SDIO_D2      (pin_C10)
#define MICROPY_HW_SDIO_D3      (pin_C11)
#define MICROPY_HW_SDIO_CK      (pin_C12)
#define MICROPY_HW_SDIO_CMD     (pin_D2)
#endif*/

void sdio_init(uint32_t irq_pri) {
}

void sdio_deinit(void) {
}

void sdio_reenable(void) {
 
}

void sdio_enable_irq(bool enable) {

}

void sdio_enable_high_speed_4bit(void) {
    
}

void SDMMC_IRQHandler(void) {
    
}

#endif
