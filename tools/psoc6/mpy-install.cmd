@echo off
setlocal

call :quick_start %1 %2 

exit /b %ERRORLEVEL%

rem ~~~~~~~~~~~~~~~~
rem Script functions
rem ~~~~~~~~~~~~~~~~

:mpy_deploy_firmware

    set board=%~1
    set hex_file=%~2

    echo Deploying Micropython...
    openocd\bin\openocd.exe -s openocd\scripts -c "source [find interface/kitprog3.cfg]; ; source [find target/psoc6_2m.cfg]; psoc6 allow_efuse_program off; psoc6 sflash_restrictions 1; program %fhex_file% verify reset exit;"

exit /b 0

:mpy_download_firmware:

    set board=%~1
    set version=%~2

    @REM set mpy_hex=micropython-ifx-%board%-%mpy_ifx_version%
    set mpy_hex=firmware.hex
    echo Downloading MicroPython PSoC6 port %version% for %board% board...
    Rem curl.exe -s -L https://github.com/jaenrig-ifx/micropython/releases/download/%mpy_ifx_version%/micropython-ifx-!board!.hex > micropython-ifx-!board!.hex

exit /b 0

:mpy_clean:

    echo Cleaning up micropython hex files...
    Rem del micropython-ifx-%board%.hex

exit /b 0

:openocd_download_install

    echo Downloading openocd...
    curl.exe -s -L https://github.com/Infineon/openocd/releases/download/release-v4.4.0/openocd-4.4.0.2134-windows.zip > openocd.zip
    echo Extracting openocd...
    tar.exe -xf openocd.zip

exit /b 0

:openocd_uninstall_clean

    echo Cleaning up openOCD installation package...
    del openocd.zip 
    rmdir /s /q openocd

exit /b 0

:arduino_lab_download_and_launch

    echo Downloading Arduino Lab for Micropython...
    curl.exe -s -L https://github.com/arduino/lab-micropython-editor/releases/download/0.5.0-alpha/Arduino.Lab.for.Micropython-win_x64.zip > arduino-for-micropython.zip
    echo Extracting Arduino Lab for Micropython...
    tar.exe -xf arduino-for-micropython.zip
    echo Launching Arduino Lab for Micropython...
    start "" "Arduino Lab for Micropython"

exit /b 0

:arduino_lab_install_package_clean

    echo Cleaning up Arduino Lab for Micropython installation package...
    del arduino-for-micropython.zip

exit /b 0

:mpy_setup_device

    setlocal enabledelayedexpansion

    Rem Board selection
    set board=%~1
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
    set mpy_firmware_version=%~2
    if [%mpy_firmware_version%]==[] set mpy_firmware_version=latest
    echo MicroPython PSoC6 Version :: %mpy_firmware_version% 
    echo:

    Rem Download flashing tool and firmware
    call :openocd_download_install
    call :mpy_download_firmware %board% %mpy_firmware_version%

    if not [%~3]==[\q] (
        echo:
        echo Please CONNECT THE BOARD and PRESS ANY KEY to start the firmware deployment...
        pause >nul
        echo:
    )

    Rem Deploy on board
    call :mpy_deploy_firmware %board% %mpy_firmware_version%
    echo Device firmware deployment completed.   

    call :openocd_uninstall_clean
    call :mpy_clean

    if not [%~3]==[\q] (
        echo:
        echo Press any key to continue...
        pause >nul
        echo:
    )


exit /b 0

:quick_start

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
    echo                  Quick Start
    echo ################################################

    call :mpy_setup_device %1 %2

    call :arduino_lab_download_and_launch

    call :arduino_lab_install_package_clean

    echo:   
    echo ###########################################
    echo  The installation is completed. Have fun :)                 
    echo ###########################################     

    echo:
    echo Press any key to exit...
    pause >nul

exit /b 0
