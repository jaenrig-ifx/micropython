
# get path to this file
MPY_PATH_TO_MTB_ADAPTER_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
MPY_DIR_OF_MTB_ADAPTER_MAKEFILE  := $(dir $(MPY_PATH_TO_MTB_ADAPTER_MAKEFILE))


# get variable definitions from main makefile
MPY_MTB_MAIN_MAKEFILE := $(MPY_DIR_OF_MTB_ADAPTER_MAKEFILE)/Makefile
MPY_MTB_TARGET        := $(shell egrep '^ *TARGET' $(MPY_MTB_MAIN_MAKEFILE) | sed 's/^.*= *//g')
MPY_MTB_CONFIG        := $(shell egrep '^ *CONFIG' $(MPY_MTB_MAIN_MAKEFILE) | sed 's/^.*= *//g')
MPY_MTB_APPNAME       := $(shell egrep '^ *APPNAME' $(MPY_MTB_MAIN_MAKEFILE) | sed 's/^.*= *//g')

MPY_MTB_BOARD_BUILD_DIR        := $(MPY_DIR_OF_MTB_ADAPTER_MAKEFILE)/$(BUILD)
MPY_MTB_BOARD_BUILD_OUTPUT_DIR := $(MPY_MTB_BOARD_BUILD_DIR)/$(MPY_MTB_TARGET)/$(MPY_MTB_CONFIG)


ifdef FORCE_MTB_BUILD
  MPY_REMOVE_MTB_BUILD_DIR := rm -rf $(MPY_MTB_BOARD_BUILD_DIR) ;
else
  MPY_REMOVE_MTB_BUILD_DIR :=
endif


$(info MPY_PATH_TO_MTB_ADAPTER_MAKEFILE : $(MPY_PATH_TO_MTB_ADAPTER_MAKEFILE))
$(info MPY_DIR_OF_MTB_ADAPTER_MAKEFILE  : $(MPY_DIR_OF_MTB_ADAPTER_MAKEFILE))

$(info MPY_MTB_MAIN_MAKEFILE            : $(MPY_MTB_MAIN_MAKEFILE))
$(info MPY_MTB_TARGET                   : $(MPY_MTB_TARGET))
$(info MPY_MTB_CONFIG                   : $(MPY_MTB_CONFIG))
$(info MPY_MTB_APPNAME                  : $(MPY_MTB_APPNAME))

$(info MPY_MTB_BOARD_BUILD_DIR          : $(MPY_MTB_BOARD_BUILD_DIR))
$(info MPY_MTB_BOARD_BUILD_OUTPUT_DIR   : $(MPY_MTB_BOARD_BUILD_OUTPUT_DIR))
$(info MPY_REMOVE_MTB_BUILD_DIR         : $(MPY_REMOVE_MTB_BUILD_DIR))


# init MTB project
mpy_mtb_init:
	$(info )
	$(info Initializing $(BOARD) ...)
	-$(Q) cd $(BOARD_DIR); $(MAKE) getlibs


# build MTB project
mpy_mtb_build: mpy_mtb_init
	$(info )
	$(info Building $(BOARD) using MTB ...)
	-$(Q) cd $(BOARD_DIR); $(MPY_REMOVE_MTB_BUILD_DIR) $(MAKE) build

# added the following flag to MTB Makefile in order to reduce recomile runsS
# DEFINES+=CY_RETARGET_IO_CONVERT_LF_TO_CRLF=1


# get variables set for includes, objects, etc. and add to mpy makefile variables
#
# TODO : set CFLAGS and CXXFLAGS using .cycompiler by filtering out the gcc command. Currently this file contains multiple concatenated compile commands, so filtering has to be applied !
#	     $(eval ... = $(file < $(MPY_DIR_OF_MTB_ADAPTER_MAKEFILE)/build/$(MPY_MTB_TARGET)/$(MPY_MTB_CONFIG)/.cycompiler))
# TODO: set LD using .cylinker once the bugs (superfluous -o option, erroneous call to ar)
#       $(eval LD = $(file < $(MPY_DIR_OF_MTB_ADAPTER_MAKEFILE)/build/$(MPY_MTB_TARGET)/$(MPY_MTB_CONFIG)/.cylinker))
# TODO: linking using the MTB requires a main program, otherwise linking fails. Therefore leave main.c in the MTB folder and filter it out. Since there are so
#       many files in the MTB setup using the line below may be better, otherwise on certain platforms the line may become too long and compilation fails. So better to use this
#       line to set LDFLAGS :
#	$(eval LDFLAGS += -mcpu=cortex-m4 --specs=nano.specs -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -ffunction-sections -fdata-sections -ffat-lto-objects -g -Wall -pipe -Wl,--gc-sections -Tboards/$(MPY_MTB_BOARD_NAME)/bsps/TARGET_APP_CY8CPROTO-062-4343W/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/linker.ld -Wl,-Map,/home/jensb/micropython/ports/minimal/build/firmware.map @/home/jensb/micropython/ports/minimal/boards/$(MPY_MTB_BOARD_NAME)/build/APP_CY8CPROTO-062-4343W/Debug/objlist.rsp -Wl,--start-group -Wl,--end-group )

mpy_define_mtb_vars: mpy_mtb_build
	$(info )
	$(eval MPY_MTB_INCLUDE_DIRS = $(file < $(MPY_MTB_BOARD_BUILD_OUTPUT_DIR)/inclist.rsp))
	$(info MPY_MTB_INCLUDE_DIRS : $(MPY_MTB_INCLUDE_DIRS))
	$(info )
	$(eval INC += $(subst -I,-I$(BOARD_DIR)/,$(MPY_MTB_INCLUDE_DIRS)))
	$(info )
	$(eval MPY_MTB_ALL_OBJECTS = $(file < $(MPY_MTB_BOARD_BUILD_OUTPUT_DIR)/objlist.rsp))
	$(eval MPY_MTB_OBJECTS = $(filter-out %/main.o,$(MPY_MTB_ALL_OBJECTS)))
	$(info MPY_MTB_OBJECTS : $(MPY_MTB_OBJECTS))
	$(info )
	$(eval OBJ += $(MPY_MTB_OBJECTS))
	$(info )
	$(eval MPY_MTB_LIBRARIES=$(file < $(MPY_MTB_BOARD_BUILD_OUTPUT_DIR)/liblist.rsp))
	$(info MPY_MTB_LIBRARIES : $(MPY_MTB_LIBRARIES))
	$(info )
	$(eval LIBS += $(filter-out %/main.o,$(MPY_MTB_LIBRARIES)))
	$(info )
	$(eval CFLAGS += -mcpu=cortex-m4 --specs=nano.specs -Og -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -ffunction-sections -fdata-sections -ffat-lto-objects -g -Wall -pipe -DCOMPONENT_4343W -DCOMPONENT_APP_CY8CPROTO_062_4343W -DCOMPONENT_CAT1 -DCOMPONENT_CAT1A -DCOMPONENT_CM0P_SLEEP -DCOMPONENT_CM4 -DCOMPONENT_CM4_0 -DCOMPONENT_Debug -DCOMPONENT_GCC_ARM -DCOMPONENT_HCI_UART -DCOMPONENT_MURATA_1DX -DCOMPONENT_MW_CAT1CM0P -DCOMPONENT_MW_CMSIS -DCOMPONENT_MW_CORE_LIB -DCOMPONENT_MW_CORE_MAKE -DCOMPONENT_MW_MTB_HAL_CAT1 -DCOMPONENT_MW_MTB_PDL_CAT1 -DCOMPONENT_MW_RECIPE_MAKE_CAT1A -DCOMPONENT_MW_RETARGET_IO -DCOMPONENT_PSOC6_02 -DCOMPONENT_SOFTFP -DCOMPONENT_WIFI_INTERFACE_SDIO -DCORE_NAME_CM4_0=1 -DCY8C624ABZI_S2D44 -DCY_APPNAME_mtb_example_hal_hello_world -DCY_SUPPORTS_DEVICE_VALIDATION -DCY_TARGET_BOARD=APP_CY8CPROTO_062_4343W -DCY_USING_HAL -DDEBUG -DTARGET_APP_CY8CPROTO_062_4343W)
	$(info )
	$(eval CXXFLAGS += $(CFLAGS))
	$(info )
	$(eval LDFLAGS += -mcpu=cortex-m4 --specs=nano.specs -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -ffunction-sections -fdata-sections -ffat-lto-objects -g -Wall -pipe -Wl,--gc-sections -T$(MPY_DIR_OF_MTB_ADAPTER_MAKEFILE)/bsps/TARGET_APP_CY8CPROTO-062-4343W/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/linker.ld -Wl,-Map,$(BUILD)/firmware.map $(MPY_MTB_OBJECTS) -Wl,--start-group -Wl,--end-group )
	$(info )
	$(info LD : $(LD))
	$(info )
	$(eval QSTR_GEN_CFLAGS += $(INC))
	touch mpy_define_mtb_vars


mpy_program: $(MPY_MAIN_BUILD_DIR)/firmware.elf
	$(info )
	$(info Replacing firmware.hex MTB build folder)
	$(Q) cp $(MPY_MAIN_BUILD_DIR)/firmware.elf $(MPY_MTB_BOARD_BUILD_OUTPUT_DIR)/$(MPY_MTB_APPNAME).elf
	$(info Programming using MTB programmer)
	$(Q) cd $(BOARD_DIR); $(MAKE) program


.PHONY: mpy_mtb_init mpy_mtb_build mpy_define_mtb_vars mpy_program