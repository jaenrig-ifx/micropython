@echo off

set board=%1
if [%board%]==[] echo No board defined
echo MicroPython PSoC Board   :: %board%


set mpy_ifx_version=%2
if [%mpy_ifx_version%]==[] set mpy_ifx_version=latest

echo Micropython PSoC Version :: %mpy_ifx_version% 


echo Downloading MicroPython PSoC6 port %mpy_ifx_version% for %board% board...
Rem curl.exe -s -L https://github.com/jaenrig-ifx/micropython/releases/download/%mpy_ifx_version%/micropython-ifx-%board%.hex > micropython-ifx-%board%.hex

echo Downloading openocd...
curl.exe -s -L https://github.com/Infineon/openocd/releases/download/release-v4.4.0/openocd-4.4.0.2134-windows.zip > openocd.zip

echo Extracting openocd...
tar.exe -xf openocd.zip

echo:
echo Please CONNECT THE BOARD and PRESS ANY KEY to start the firmware deployment...
pause >nul

echo Deploying Micropython...
Rem openocd/bin/openocd.exe -s openocd/scripts -c "source [find interface/kitprog3.cfg]; ; source [find target/psoc6_2m.cfg]; psoc6 allow_efuse_program off; psoc6 sflash_restrictions 1; program micropython-ifx-%board%.hex verify reset exit;"
Rem /home/jaenrig/ModusToolbox/tools_3.0/openocd/bin/openocd -s /home/jaenrig/ModusToolbox/tools_3.0/openocd/scripts -s bsps/TARGET_APP_CY8CPROTO-062-4343W/config/GeneratedSource -c "source [find interface/kitprog3.cfg]; ; source [find target/psoc6_2m.cfg]; psoc6 allow_efuse_program off; psoc6 sflash_restrictions 1; program $(BUILD)/firmware.hex verify reset exit;"

echo Cleaning up downloaded files...
Rem del micropython-ifx-%board%.hex
del openocd.zip 
rmdir /s /q openocd

echo  __  __ _            ___      _   _             
echo ^|  \/  (_)__ _ _ ___^| _ \_  _^| ^|_^| ^|_  ___ _ _ 
echo ^| ^|\/^| ^| / _^| '_/ _ \  _/ ^|^| ^|  _^| ' \/ _ \ ' \ 
echo ^|_^|_ ^|_^|_\__^|_^| \___/_^|  \_, ^|\__^|_^|^|_\___/_^|^|_^|
echo ^| _ \/ __^| ___ / __^|/ /  ^|__/                    
echo ^|  _/\__ \/ _ \ (__/ _ \                         
echo ^|_^|  ^|___/\___/\___\___/  
echo:      
echo installation is ready. Enjoy!                 
                                                  
echo:
pause	