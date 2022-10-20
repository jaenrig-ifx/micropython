# PSoC6 port

This port is intended to run on Infineon PSoC™ 6 microcontrollers.

## Pre-requisites
 
The following port is using Infineon ModusToolbox™ to resolve the specific PSoC™ board resources and building flags. Before working with micropython:

1. Install [ModusToolbox](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/). Minimum version required is 3.0.
2. Add `project-creator-cli` to the system PATH. This tool is usually installed in *{ModusToolbox™ software install directory}/tools_{version}/project-creator/*. In your Linux terminal, for the default installation path of ModusToolbox™(only for the current terminal session):

    $ export PATH=$PATH:/home/{user}/ModusToolbox/tools_{version}/project-creator

## Building and running Linux version

As we are working on the ports/psoc6 branch (for now), first checkout that branch after cloning this repo:

    $ git checkout --track origin/ports/psoc6

Then initialize the ModusToolbox™ environment: 

    $ make mtb-init

Build the firmare:

    $ make

To build and program the device:

    $ make program

### Working from WSL

If you are working from WSL and you cannot access the USB resources of your machine. Copy the .hex file from *ports/psoc6/boards/{board}/mtb-app/APP_{board}/Debug/{hexname}.hex* to some location in your windows machine where you can flash the device with Cypress Programmer. 

# Run micropython

Use any serial terminal (putty, minicom..) and stablish a session with your device with 115200 bauds and 8-N-1 configuration. 
