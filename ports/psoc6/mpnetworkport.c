/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Infineon Technologies AG
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


#if MICROPY_PY_NETWORK_IFX_WHD

#include "mphalport.h"

/*********************************************** Semaphores **********************************************/
// Empty implementation of semaphores required by the whd driver.
// as long as no RTOS support is added to the PSoC6 port.

#include "cyabs_rtos.h"

cy_rslt_t cy_rtos_init_semaphore(cy_semaphore_t *semaphore, uint32_t maxcount, uint32_t initcount) {
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_rtos_get_semaphore(cy_semaphore_t *semaphore, cy_time_t timeout_ms, bool in_isr) {
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_rtos_set_semaphore(cy_semaphore_t *semaphore, bool in_isr) {
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_rtos_deinit_semaphore(cy_semaphore_t *semaphore) {
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_rtos_get_time(cy_time_t *tval) {
    *tval = (cy_time_t)mp_hal_ticks_ms();
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_rtos_delay_milliseconds(cy_time_t num_ms) {

    mp_hal_delay_ms(num_ms);
    return CY_RSLT_SUCCESS;
}

#endif // MICROPY_PY_NETWORK_IFX_WHD
