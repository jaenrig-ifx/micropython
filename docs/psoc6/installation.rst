.. _psoc6_mpy_install:

Installing MicroPython 
======================

To support the MicroPython PSoC6™ port installation the ``mpy-psoc6`` utility script is provided for Windows and
Linux.
You can easily download from your OS terminal with the following command:

.. tabs::

    .. group-tab:: Linux

        .. code-block:: bash

            curl -s -L https://raw.githubusercontent.com/jaenrig-ifx/micropython/ports/psoc6/tools/psoc6/mpy-psoc6.sh > mpy-psoc6.sh 

        Add execution rights to the script:       
        
        .. code-block:: bash                
           
            chmod +x mpy-psoc6.sh 

    .. group-tab:: Windows
    
        Download the mpy-psoc6 utility script:

            .. code-block:: bash

                curl.exe -s -L https://raw.githubusercontent.com/jaenrig-ifx/micropython/ports/psoc6/tools/psoc6/mpy-psoc6.cmd > mpy-psoc6.cmd


Find all the available commands an options by running the script with the command help:

.. tabs::

    .. group-tab:: Linux

        .. code-block:: bash

            ./mpy-psoc6.sh help

    .. group-tab:: Windows

            .. code-block:: bash
            
                ./mpy-psoc6.sh help

.. _psoc6_quick_start:

Quick Start
------------

With the ``mpy-psoc6`` utility script donwloaded, the fastest way to get you up and running with
micropython is to run the ``quick-start`` command of the script:

.. tabs::

    .. group-tab:: Linux
        
            .. code-block:: bash

                ./mpy-psoc6.sh quick-start

    .. group-tab:: Windows

            .. code-block:: bash
                
                mpy-psoc6.cmd quick-start

This will seamlessly take care of the following:

* Install all required software to work with MicroPython
* Deploy the latest version of MicroPython PSoC6 firmware on your board
* Launch Arduino Lab MicroPython IDE
* Remove all installation packages and software

This command is supporting the getting started tutorial for the first time. Once you get familiar
with MicroPython and its environment, the ``device-setup`` command will be more appropiate to
install MicroPython on PSoC6™ boards, and upgrade your device for newer 

Device setup
-------------

In order to setup MicroPython in a PSoC6™ board, the ``device-setup`` command of the ``mpy-psoc6``
can be executed. Follow the instructions to select the target PSoC6™ board, and deploy the latest
MicropPython firmware version:

.. tabs::

    .. group-tab:: Linux
        
            .. code-block:: bash

                ./mpy-psoc6.sh setup-device

    .. group-tab:: Windows

            .. code-block:: bash
                
                mpy-psoc6.cmd setup-device


You can run any command any time you want to upgrade to the latest MicroPython firmware version.
This command will take care of the following steps:

* Download and install openocd, which is the software required to deploy a firmware file on PSoC6™ controllers
* Download the latest ``.hex`` file for your select board
* Deploy the firmware on your board
* Remove all installed and donwloaded files

Updating the device firmware
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Install a specific versions
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Getting the firmware
^^^^^^^^^^^^^^^^^^^^

The binary *.hex* file is available in the `GitHub release section <https://github.com/jaenrig-ifx/MicroPython/releases>`_. 
All PSoC6™ firmware versions for each of the supported boards can be found there. 

..
    .. image:: img/gh-releases.jpg
..
    :alt: GitHub MicroPython Releases
..
    :width: 520px

Other installation methods
--------------------------

Cypress Programmer
^^^^^^^^^^^^^^^^^^

In order to deploy the firmware in the embedded device, the easiest and quickest way is to use the
`Cypress Programmer <https://softwaretools.infineon.com/tools/com.ifx.tb.tool.cypressprogrammer>`_.
It allows to flash a binary in the PSoC6™ microcontrollers family in a few clicks from your Windows,
Linux or Mac OS machine. 
Follow the instructions on the provided link to download and install the tool.

After that, select the downloaded MicroPython firmware *.hex* file to be deployed on the PSoC6™. Then, in
the upper menu, select the connected *Probe/Kit*, click on *Connect*, and finally click on *Program*.
The log section will show the progress and inform when the firmware deployment on the controller is completed.

.. image:: img/cy-programmer.jpg
    :alt: Cypress Programmer GUI
    :width: 520px

For a detailed description on how to use the Cypress Programmer tool, please consult the `Cypress
Programmer User Guide
<https://www.infineon.com/dgdl/Infineon-Infineon_Programmer_4.0_GUI_User_Guide-Software-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017ed9abca6e365c>`_.

You are all set now to start programming with MicroPython!

ModusToolbox™
^^^^^^^^^^^^^

.. note::
    Alternatively, users familiar with the ModusToolbox™ toolchain and makefile system can also use the corresponding
    tools for flashing the provided *.hex* file on the target PSoC6™ board. 


