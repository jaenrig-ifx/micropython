from machine import Pin
import time

# Tests for Output pin
# Instantiate output pin for led operation
p1 = Pin("P13_7")
print(p1)
p1.init(Pin.OUT)
p1.value(0)
print(p1.value())
p1.value(1)
print(p1.value())
p1.value(False)
print(p1.value())
p1.value(True)
print(p1.value())

# Tests for Input pin
# Instantiate input pin for button operation
p2 = Pin("P0_4", Pin.IN)
print(p2)
p2.value()
print(p2.value()) # Should be 1 - button is released

p2.value()
print(p2.value()) # Should be 0 - button is pressed
