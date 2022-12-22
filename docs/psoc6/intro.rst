.. _psoc6_intro:

Getting started with MicroPython on the PSoC6™
==============================================

This tutorial will guide you to get started with running MicroPython on the PSoC6™ microcontrollers. 
There are only a few step keeping you away from enjoying the python programming experience together
with the possibilities of PSoC6™ microcontrollers.
Let's get started!

Requirements
------------

The only required hardware is:

* PSoC6™ board of the :ref:`Supported boards` list.
* A microUSB cable.

Powering the board
------------------

Connect the USB cable to your computer and the micro USB to the board debugger. All the PSoC6™ boards
come with a on-board debugger required for flashing/debugging operations during development. Please refer to the
corresponding board manual of your board.

Install MicroPython
-------------------

In order to simply things, you can run the ``quick-start`` command of the mpy-psoc6 utility. In your
machine terminal type the following commands. Follow the instructions on the terminal:

.. tabs::

    .. group-tab:: Linux

        .. code-block:: bash

            curl -s -L https://raw.githubusercontent.com/jaenrig-ifx/micropython/ports/psoc6/tools/psoc6/mpy-psoc6.sh > mpy-psoc6.sh 

        Add execution rights to the script and run the script:       
        
        .. code-block:: bash                
           
            chmod +x mpy-psoc6.sh 
            ./mpy-psoc6.sh quick-start

    .. group-tab:: Windows
    
        Download the mpy-psoc6 utility script:

            .. code-block:: bash

                curl.exe -s -L https://raw.githubusercontent.com/jaenrig-ifx/micropython/ports/psoc6/tools/psoc6/mpy-psoc6.cmd > mpy-psoc6.cmd
                mpy-psoc6.cmd quick-start

If everything went fine, your PSoC6™ board is now running MicroPython and Arduino IDE for
Micropython is now started.

Select your serial port of your PSoC6™ board by clicking on the connect icon on the menu bar:



Interact with the MicroPython prompt
------------------------------------



Run your first script
---------------------



Upload a script to your device
------------------------------


