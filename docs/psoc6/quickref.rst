.. _psoc6_quickref:

Quick reference for the PSoC6™
==============================

.. image:: img/cy8cproto-062-4343w.jpg
    :alt: CY8CPROTO-062-4343W board
    :width: 640px

The CY8CPROTO-062-4343W PSoC6™ Board.

Below is a quick reference for PSoC6-based boards.  If it is your first time
working with this board it may be useful to get an overview of the microcontroller:

.. toctree::
   :maxdepth: 1

   general.rst
   tutorial/index.rst

Installing MicroPython
----------------------

See the corresponding section of tutorial: :ref:`psoc6_intro`.

General board control
---------------------

The MicroPython REPL is accessed via the USB serial port. Paste mode (ctrl-E) is useful to paste a
large slab of Python code into the REPL. 

This port implements most of the methods described in the :mod:`machine` module. Tab-completion is useful to
find out what methods an instantiated object has. 

The :mod:`machine` module::

    import machine

    machine.freq()          # get the current frequency of the CPU
    
..
    TODO: add more machine module examples when implemented.    
    machine.freq(240000000) # set the CPU frequency to 240 MHz

..
    TODO: add ``psoc6`` module when implemented.    


Delay and timing
----------------

Timers
------

Pins and GPIO
-------------

Most of the methods (functions) and constants given in the :ref:`machine.Pin <machine.Pin>` class have been implemented in this port. Any functions in addition to those or function calls with ambiguous list of parameters have been documented here with suitable examples. 

The constructor
^^^^^^^^^^^^^^^
The constructor can be called in different flavors and configurations based on the number of arguments (parameters) passed. 

An instance of the :mod:`machine.Pin` class can be created by invoking the constructor with all the necessary parameters to fully configure the ``Pin``. 

::

    from machine import Pin

    p0 = Pin('P13_7', Pin.OUT, Pin.PULL_DOWN, value=STATE_LOW)   # create output pin on pin P13_7, 
                                                                 # with pull-down resistor enabled,
                                                                 # with initial value 0 (STATE_LOW)     


Additionally, with any combination of parameters (except the Pin number or ``id`` which should be passed mandatorily), a :mod:`machine.Pin` object with various configuration levels can be instantiated. In these cases, the :meth:`Pin.init` function has to be called proactively to set the other necessary configurations, as needed.

Moreover, a pre-configured pin object can be repurposed by calling the :meth:`Pin.init` function.


::

    from machine import Pin

    p0 = Pin('P13_7')                    # create pin object for pin P13_7. 
    p0.init(p0.OUT, p0.PULL_DOWN)        # set pin as output and enable pull-down resistor.
    p0.low()                             # set value low.     




Similar to CPython, the parameters can be passed in any order if keywords are used. On the other hand, in case of a non-keyword assignment if a parameter is not to be set, a ``None`` is to be passed in its place.


::

    from machine import Pin

    p0 = Pin(id='P13_7', value=STATE_LOW, pull=Pin.PULL_DOWN, mode=Pin.OUT)     # create output pin on pin P13_7, 
                                                                                # with pull-down resistor enabled,
                                                                                # with initial value 0 (STATE_LOW) 


    p1 = Pin('P0_0', Pin.OUT, None, value=STATE_HIGH)                           # create output pin on pin P0_0, 
                                                                                # with pull as NONE,
                                                                                # with initial value 1 (STATE_HIGH)                                                                       

Note that the parameters such as ``value`` can only be passed as keyword arguments.  

..
    TODO: add ``drive`` and ``alt`` when implemented

Methods
^^^^^^^

.. method:: Pin.toggle()

   Set pin value to its complement.


Constants
^^^^^^^^^
The following constants are used to configure the pin objects in addition to the ones mentioned in the :mod:`machine.Pin` class.

.. data:: Pin.STATE_LOW
          Pin.STATE_HIGH
          
    Selects the pin value.

Software I2C bus
-------------
Software I2C (using bit-banging) works on all output-capable pins, and is
accessed via the :ref:`machine.SoftI2C <machine.SoftI2C>` class::

    from machine import Pin, SoftI2C

    i2c = SoftI2C(scl='P5_0', sda='P5_1', freq=100000)

    i2c.scan()              # scan for devices

    i2c.readfrom(0x3a, 4)   # read 4 bytes from device with address 0x3a
    i2c.writeto(0x3a, '12') # write '12' to device with address 0x3a

    buf = bytearray(10)     # create a buffer with 10 bytes
    i2c.writeto(0x3a, buf)  # write the given buffer to the peripheral

Hardware I2C bus
--------------
Hardware I2C works on the following listed pair of I2C pins

=====  ===========  ============
\      Default  
=====  ===========  ============
scl    P6_0           P9_0
sda    P6_1           P9_1
=====  ===========  ============


The driver is accesses via the :ref:`machine.I2C <machine.I2C>` class::

The constructor
^^^^^^^^^^^^^^^
An instance of the :mod:`machine.I2C` class can be created by invoking the constructor with all the 
necessary parameters to fully configure the ``I2C``. By invoking the constructor I2C peripheral is 
initialized and configured to work in master mode.

i2c = I2C(0,scl='P6_0',sda='P6_1',freq=4000000)

Here i2c_id=0 should be passed mandatorily which selects the master mode operation.

: :

    from machine import Pin,I2C
    i2c = I2C(0)                  #I2C is initialized & configured with default scl, sda pin & frequency

: :

    from machine import Pin,I2C
    i2c = I2C(0,scl='P9_0',sda='P9_1',freq=4000000)   #I2C is initialised & configured with given scl,sda pins & frequency

Methods
^^^^^^^
All the methods(functions) given in :ref:`machine.I2C <machine.I2C>` class:: have been implemented in this port except

..method:: I2C.init()
