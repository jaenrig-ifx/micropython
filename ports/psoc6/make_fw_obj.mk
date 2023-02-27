TOP = ../../


# Firmware stuff
FW = WA1

ifeq ($(FW),WA1)

CYW43_FIRMWARE_BIN      = 4343WA1-7.45.98.50.combined
CYW43_FIRMWARE_BIN_     = 4343WA1_7_45_98_50_combined
CYW43_FIRMWARE_PRETTY   = fw_4343WA1_7_45_98_50

else

CYW43_FIRMWARE_BIN      = 43439A0-7.95.49.00.combined
#CYW43_FIRMWARE_BIN_     = $(subst \\,_,$(CYW43_FIRMWARE_BIN))
#CYW43_FIRMWARE_BIN_     = $(subst \.,_,$(CYW43_FIRMWARE_BIN))
#CYW43_FIRMWARE_BIN_     = $(subst \-,_,$(CYW43_FIRMWARE_BIN))
CYW43_FIRMWARE_BIN_     = 43439A0_7_95_49_00_combined
CYW43_FIRMWARE_PRETTY   = fw_43439A0_7_95_49_00

endif


CYW43_FIRMWARE_BIN_PATH = $(TOP)/lib/cyw43-driver/firmware

$(info CYW43_FIRMWARE_BIN_PATH : $(CYW43_FIRMWARE_BIN_PATH))

$(info CYW43_FIRMWARE_BIN      : $(CYW43_FIRMWARE_BIN))
$(info CYW43_FIRMWARE_BIN_     : $(CYW43_FIRMWARE_BIN_))
$(info CYW43_FIRMWARE_PRETTY   : $(CYW43_FIRMWARE_PRETTY))


OBJCOPY = arm-none-eabi-objcopy
# cyw43_resource.o contains the WiFi and BT firmware as a binary blob
#
all:
	$(info hello)
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm --readonly-text \
               --rename-section .data=.big_const,contents,alloc,load,readonly,data \
               --redefine-sym _binary________lib_cyw43_driver_firmware_$(CYW43_FIRMWARE_BIN_)_start=$(CYW43_FIRMWARE_PRETTY)_start \
               --redefine-sym _binary________lib_cyw43_driver_firmware_$(CYW43_FIRMWARE_BIN_)_end=$(CYW43_FIRMWARE_PRETTY)_end \
               --redefine-sym _binary________lib_cyw43_driver_firmware_$(CYW43_FIRMWARE_BIN_)_size=$(CYW43_FIRMWARE_PRETTY)_size \
               $(CYW43_FIRMWARE_BIN_PATH)/$(CYW43_FIRMWARE_BIN) cyw43_firmware.o
