@echo off
setlocal enabledelayedexpansion

echo ################################################
echo                 Welcome to the 
echo  __  __ _            ___      _   _             
echo ^|  \/  (_)__ _ _ ___^| _ \_  _^| ^|_^| ^|_  ___ _ _ 
echo ^| ^|\/^| ^| / _^| '_/ _ \  _/ ^|^| ^|  _^| ' \/ _ \ ' \ 
echo ^|_^|_ ^|_^|_\__^|_^| \___/_^|  \_, ^|\__^|_^|^|_\___/_^|^|_^|
echo ^| _ \/ __^| ___ / __^|/ /  ^|__/                    
echo ^|  _/\__ \/ _ \ (__/ _ \                         
echo ^|_^|  ^|___/\___/\___\___/ 
echo:
echo                    Installer
echo ################################################

Rem Board selection
set board=%1
set board_list[0]=CY8CPROTO-062-4343W

if [%board%]==[] (
    echo:
    echo       Supported MicroPython PSoC6 boards
    echo +---------+-----------------------------------+
    echo ^|   ID    ^|              Board                ^|
    echo +---------+-----------------------------------+
    echo ^|   0     ^|  CY8CPROTO-062-4343W (default^)    ^|
    echo +---------+-----------------------------------+
    echo:
    echo No user selection required. Only one choice.
	set /a board_index=0
	echo:
    Rem set  /p/( "board_index=Please type the desired board ID. " --> Uncomment and remove preselection above when more options are available
    call set board=%%board_list[!board_index!]%%
)
echo MicroPython PSoC6 Board  :: %board%

Rem Version selection
set mpy_ifx_version=%2
if [%mpy_ifx_version%]==[] set mpy_ifx_version=latest

echo MicroPython PSoC6 Version :: %mpy_ifx_version% 
echo:

echo Downloading MicroPython PSoC6 port %mpy_ifx_version% for %board% board...
Rem curl.exe -s -L https://github.com/jaenrig-ifx/micropython/releases/download/%mpy_ifx_version%/micropython-ifx-!board!.hex > micropython-ifx-!board!.hex

echo Downloading openocd...
curl.exe -s -L https://github.com/Infineon/openocd/releases/download/release-v4.4.0/openocd-4.4.0.2134-windows.zip > openocd.zip

echo Extracting openocd...
tar.exe -xf openocd.zip

echo:
echo Please CONNECT THE BOARD and PRESS ANY KEY to start the firmware deployment...
pause >nul

echo Deploying Micropython...
openocd\bin\openocd.exe -s openocd\scripts -c "source [find interface/kitprog3.cfg]; ; source [find target/psoc6_2m.cfg]; psoc6 allow_efuse_program off; psoc6 sflash_restrictions 1; program firmware.hex verify reset exit;"

@REM echo:
@REM echo Downloading Arduino Lab for Micropython...
@REM curl.exe -s -L https://github.com/arduino/lab-micropython-editor/releases/download/0.5.0-alpha/Arduino.Lab.for.Micropython-win_x64.zip > arduino-for-micropython.zip

@REM echo Extracting Arduino Lab for Micropython...
@REM tar.exe -xf arduino-for-micropython.zip

@REM echo Launching Arduino Lab for Micropython...
@REM start "" "Arduino Lab for Micropython"

echo:
echo Cleaning up downloaded files...
Rem del micropython-ifx-%board%.hex
del arduino-for-micropython.zip
del openocd.zip 
rmdir /s /q openocd

echo:   
echo ###########################################
echo  The installation is completed. Have fun :)                 
echo ###########################################      

echo:
echo Press any key to exit...
pause >nul