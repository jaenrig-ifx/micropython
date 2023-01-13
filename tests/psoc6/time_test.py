'''try:
    import utime

    utime.sleep_ms, utime.sleep_us, utime.ticks_diff, utime.ticks_ms, utime.ticks_us, utime.ticks_cpu
except (ImportError, AttributeError):
    print("SKIP")
    raise SystemExit
'''
import utime
time.sleep_ms(1)
time.sleep_us(1)

t0 = time.ticks_ms()
t1 = time.ticks_ms()
print(0 <= time.ticks_diff(t1, t0) <= 1)

t0 = time.ticks_us()
t1 = time.ticks_us()
print(0 <= time.ticks_diff(t1, t0) <= 500)

# ticks_cpu may not be implemented, at least make sure it doesn't decrease
t0 = time.ticks_cpu()
t1 = time.ticks_cpu()
print(time.ticks_diff(t1, t0) >= 0)
