
# get path to this file
MPY_PATH_TO_MTB_ADAPTER_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
MPY_DIR_OF_MTB_ADAPTER_MAKEFILE  := $(dir $(MPY_PATH_TO_MTB_ADAPTER_MAKEFILE))


# get variable definitions from main makefile
MPY_MTB_MAIN_MAKEFILE := $(MPY_DIR_OF_MTB_ADAPTER_MAKEFILE)/Makefile
MPY_MTB_TARGET        := $(shell egrep '^ *TARGET' $(MPY_MTB_MAIN_MAKEFILE) | sed 's/^.*= *//g')
MPY_MTB_CONFIG        ?= $(shell egrep '^ *CONFIG' $(MPY_MTB_MAIN_MAKEFILE) | sed 's/^.*= *//g')

MPY_MTB_BOARD_BUILD_DIR        := $(MPY_DIR_OF_MTB_ADAPTER_MAKEFILE)/$(BUILD)
MPY_MTB_BOARD_BUILD_OUTPUT_DIR := $(MPY_MTB_BOARD_BUILD_DIR)/$(MPY_MTB_TARGET)/$(MPY_MTB_CONFIG)

MPY_MTB_LIB_NAME       = $(file < $(MPY_MTB_BOARD_BUILD_OUTPUT_DIR)/artifact.rsp)



# using WSL : should be something like /mnt/c/Users/???/ModusToolbox  with ??? to be replaced by user windows home directory
MTB_WIN_HOME ?= $(OLDPWD)/ModusToolbox
MTB_HOME     ?= $(HOME)/ModusToolbox

OPENOCD_HOME     ?= $(MTB_HOME)/tools_3.0/openocd
OPENOCD_WIN_HOME ?= $(MTB_WIN_HOME)/tools_3.0/openocd


$(info MPY_DIR_OF_MTB_ADAPTER_MAKEFILE  : $(MPY_DIR_OF_MTB_ADAPTER_MAKEFILE))

$(info MPY_MTB_MAIN_MAKEFILE            : $(MPY_MTB_MAIN_MAKEFILE))
$(info MPY_MTB_TARGET                   : $(MPY_MTB_TARGET))
$(info MPY_MTB_CONFIG                   : $(MPY_MTB_CONFIG))
$(info MTB_LIB_NAME                     : $(MPY_MTB_LIB_NAME))

$(info MPY_MTB_BOARD_BUILD_DIR          : $(MPY_MTB_BOARD_BUILD_DIR))
$(info MPY_MTB_BOARD_BUILD_OUTPUT_DIR   : $(MPY_MTB_BOARD_BUILD_OUTPUT_DIR))


# init MTB project
mpy_mtb_init:
	$(info )
	$(info Initializing $(BOARD) ...)
	-$(Q) cd $(BOARD_DIR); $(MAKE) getlibs


# build MTB project
mpy_mtb_build:
	$(info )
	$(info Building $(BOARD) in $(CONFIG) mode using MTB ...)
	-$(Q) cd $(BOARD_DIR); $(MAKE) CONFIG=$(MPY_MTB_CONFIG) build


mpy_mtb_clean:
	$(info )
	$(info Removing $(MPY_MTB_BOARD_BUILD_DIR) ...)
	-$(Q) rm -rf $(MPY_MTB_BOARD_BUILD_DIR)


# get variables set for includes, objects, etc. and add to mpy makefile variables
mpy_define_mtb_vars: mpy_mtb_build
	$(eval MPY_MTB_INCLUDE_DIRS = $(file < $(MPY_MTB_BOARD_BUILD_OUTPUT_DIR)/inclist.rsp))
	$(eval INC += $(subst -I,-I$(BOARD_DIR)/,$(MPY_MTB_INCLUDE_DIRS)))
#	$(info INC : $(INC))
	$(eval MPY_MTB_LIBRARIES = $(file < $(MPY_MTB_BOARD_BUILD_OUTPUT_DIR)/liblist.rsp))
#	$(info MPY_MTB_LIBRARIES : $(MPY_MTB_LIBRARIES))
	$(eval LIBS += $(MPY_MTB_LIBRARIES) $(MPY_MTB_BOARD_BUILD_OUTPUT_DIR)/$(MPY_MTB_LIB_NAME))
#	$(info LIBS : $(LIBS))
	$(eval CFLAGS += -mcpu=cortex-m4 --specs=nano.specs -Og -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -ffunction-sections -fdata-sections -ffat-lto-objects -g -Wall -pipe -DCOMPONENT_4343W -DCOMPONENT_APP_CY8CPROTO_062_4343W -DCOMPONENT_CAT1 -DCOMPONENT_CAT1A -DCOMPONENT_CM0P_SLEEP -DCOMPONENT_CM4 -DCOMPONENT_CM4_0 -DCOMPONENT_Debug -DCOMPONENT_GCC_ARM -DCOMPONENT_HCI_UART -DCOMPONENT_MURATA_1DX -DCOMPONENT_MW_CAT1CM0P -DCOMPONENT_MW_CMSIS -DCOMPONENT_MW_CORE_LIB -DCOMPONENT_MW_CORE_MAKE -DCOMPONENT_MW_MTB_HAL_CAT1 -DCOMPONENT_MW_MTB_PDL_CAT1 -DCOMPONENT_MW_RECIPE_MAKE_CAT1A -DCOMPONENT_MW_RETARGET_IO -DCOMPONENT_PSOC6_02 -DCOMPONENT_SOFTFP -DCOMPONENT_WIFI_INTERFACE_SDIO -DCORE_NAME_CM4_0=1 -DCY8C624ABZI_S2D44 -DCY_APPNAME_mtb_example_hal_hello_world -DCY_SUPPORTS_DEVICE_VALIDATION -DCY_TARGET_BOARD=APP_CY8CPROTO_062_4343W -DCY_USING_HAL -DDEBUG -DTARGET_APP_CY8CPROTO_062_4343W)
#	$(info CFLAGS : $(CFLAGS))
	$(eval CXXFLAGS += $(CFLAGS))
#	$(info CXXFLAGS : $(CXXFLAGS))
	$(eval LDFLAGS += -mcpu=cortex-m4 --specs=nano.specs -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -ffunction-sections -fdata-sections -ffat-lto-objects -g -Wall -pipe -Wl,--gc-sections -T$(MPY_DIR_OF_MTB_ADAPTER_MAKEFILE)/bsps/TARGET_APP_CY8CPROTO-062-4343W/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/linker.ld -Wl,-Map,$(BUILD)/firmware.map -Wl,--start-group -Wl,--end-group -Wl,--print-memory-usage)
#	$(info LDFLAGS : $(LDFLAGS))
	$(eval QSTR_GEN_CFLAGS += $(INC) $(CFLAGS))
	touch mpy_define_mtb_vars


# the following can be used without usbip
mpy_program_win: $(MPY_MAIN_BUILD_DIR)/firmware.hex
	$(info )
	$(info Programming using openocd ...)
	$(OPENOCD_WIN_HOME)/bin/openocd.exe -s $(OPENOCD_WIN_HOME)/scripts -s bsps/TARGET_APP_CY8CPROTO-062-4343W/config/GeneratedSource -c "source [find interface/kitprog3.cfg]; ; source [find target/psoc6_2m.cfg]; psoc6 allow_efuse_program off; psoc6 sflash_restrictions 1; program $(MPY_DIR_OF_MAIN_MAKEFILE)/build/firmware.hex verify reset exit;"
	$(info Programming done.)


# WSL : device needs to be attached using usbip
mpy_program_unix: $(MPY_MAIN_BUILD_DIR)/firmware.hex
	$(info )
	$(info Programming using openocd ...)
	$(OPENOCD_HOME)/bin/openocd -s $(OPENOCD_HOME)/scripts -s bsps/TARGET_APP_CY8CPROTO-062-4343W/config/GeneratedSource -c "source [find interface/kitprog3.cfg]; ; source [find target/psoc6_2m.cfg]; psoc6 allow_efuse_program off; psoc6 sflash_restrictions 1; program $(MPY_DIR_OF_MAIN_MAKEFILE)/build/firmware.hex verify reset exit;"
	$(info Programming done.)


mpy_program: mpy_program_unix


.PHONY: mpy_mtb_init mpy_mtb_build mpy_define_mtb_vars mpy_program