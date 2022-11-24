
#define RAISE_ERRNO(err_flag, error_val) \
    { if (err_flag == -1) \
      { mp_raise_OSError(error_val); } }


static inline mp_uint_t mp_hal_ticks_ms(void) {
    return 0;
}


static inline void mp_hal_set_interrupt_char(char c) {
}
