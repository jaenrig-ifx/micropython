
#MYP_CURRENT_WORKING_DIR=$(shell basename $(CURDIR))
#$(info MYP_CURRENT_WORKING_DIR : $(MYP_CURRENT_WORKING_DIR))


# get path to this file
MYP_PATH_TO_THIS_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
$(info MYP_PATH_TO_THIS_MAKEFILE : $(MYP_PATH_TO_THIS_MAKEFILE))

MYP_DIR_OF_THIS_MAKEFILE := $(dir $(MYP_PATH_TO_THIS_MAKEFILE))
$(info MYP_DIR_OF_THIS_MAKEFILE : $(MYP_DIR_OF_THIS_MAKEFILE))

MTB_APP_DIR_NAME = mtb-app
MTB_APP_DIR := $(BOARD_DIR)/$(MTB_APP_DIR_NAME)

MTB_BOARD_BUILD_DIR := $(MYP_DIR_OF_THIS_MAKEFILE)/$(MTB_APP_DIR)/build
$(info MTB_BOARD_BUILD_DIR : $(MTB_BOARD_BUILD_DIR))


# get variable definitions from main makefile
ifneq ($(wildcard $(MTB_APP_DIR)/.),)

MYP_MTB_MAKEFILE="$(MYP_DIR_OF_THIS_MAKEFILE)/$(MTB_APP_DIR)/Makefile"
$(info MYP_MTB_MAKEFILE : $(MYP_MTB_MAKEFILE))

MYP_MTB_TARGET=$(shell egrep '^ *TARGET' $(MYP_MTB_MAKEFILE) | sed 's/^.*= *//g')
$(info MYP_MTB_TARGET : $(MYP_MTB_TARGET))

MYP_MTB_CONFIG=$(shell egrep '^ *CONFIG' $(MYP_MTB_MAKEFILE) | sed 's/^.*= *//g')
$(info MYP_MTB_CONFIG : $(MYP_MTB_CONFIG))

MYP_MTB_APPNAME=$(shell egrep '^ *APPNAME' $(MYP_MTB_MAKEFILE) | sed 's/^.*= *//g')
$(info MYP_MTB_APPNAME : $(MYP_MTB_APPNAME))
endif


# MYP_MTB_BOARD_NAME=CY8CPROTO-062-4343W

hello:
	$(info "hello from hello target")

# build MTB project
mtb: 
	$(info "Building $(BOARD) using MTB ...")
	-$(Q) cd $(MTB_APP_DIR); make build
#	-$(Q) cd boards/$(MYP_MTB_BOARD_NAME) ; rm -rf build ; make build



# get variables set for includes, objects, etc. and add to mpy makefile variables
#
# TODO : set CFLAGS and CXXFLAGS using .cycompiler by filtering out the gcc command. Currently this file contains multiple concatenated compile commands, so filtering has to be applied !
#	     $(eval ... = $(file < $(MYP_DIR_OF_THIS_MAKEFILE)/build/$(MYP_MTB_TARGET)/$(MYP_MTB_CONFIG)/.cycompiler))
# TODO: set LD using .cylinker once the bugs (superfluous -o option, erroneous call to ar)
#       $(eval LD = $(file < $(MYP_DIR_OF_THIS_MAKEFILE)/build/$(MYP_MTB_TARGET)/$(MYP_MTB_CONFIG)/.cylinker))
# TODO: linking using the MTB requires a main program, otherwise linking fails. Therefore leave main.c in the MTB folder and filter it out. Since there are so
#       many files in the MTB setup using the line below may be better, otherwise on certain platforms the line may become too long and compilation fails. So better to use this
#       line to set LDFLAGS :
#	$(eval LDFLAGS += -mcpu=cortex-m4 --specs=nano.specs -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -ffunction-sections -fdata-sections -ffat-lto-objects -g -Wall -pipe -Wl,--gc-sections -Tboards/$(MYP_MTB_BOARD_NAME)/bsps/TARGET_APP_CY8CPROTO-062-4343W/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/linker.ld -Wl,-Map,/home/jensb/micropython/ports/minimal/build/firmware.map @/home/jensb/micropython/ports/minimal/boards/$(MYP_MTB_BOARD_NAME)/build/APP_CY8CPROTO-062-4343W/Debug/objlist.rsp -Wl,--start-group -Wl,--end-group )

myp_define_mtb_vars: mtb
	$(eval MYP_MTB_INCLUDE_DIRS = $(file < $(MTB_BOARD_BUILD_DIR)/$(MYP_MTB_TARGET)/$(MYP_MTB_CONFIG)/inclist.rsp))
	$(info MYP_MTB_INCLUDE_DIRS : $(MYP_MTB_INCLUDE_DIRS))
	$(info )
	$(eval INC += $(subst -I,-I$(MTB_APP_DIR)/,$(MYP_MTB_INCLUDE_DIRS)))
	$(info )
	$(eval MYP_MTB_ALL_OBJECTS = $(file < $(MTB_BOARD_BUILD_DIR)/$(MYP_MTB_TARGET)/$(MYP_MTB_CONFIG)/objlist.rsp))
	$(eval MYP_MTB_OBJECTS = $(filter-out %/main.o,$(MYP_MTB_ALL_OBJECTS)))
	$(info MYP_MTB_OBJECTS : $(MYP_MTB_OBJECTS))
	$(info )
	$(eval OBJ += $(MYP_MTB_OBJECTS))
	$(info )
	$(eval MYP_MTB_LIBRARIES=$(file < $(MTB_BOARD_BUILD_DIR)/$(MYP_MTB_TARGET)/$(MYP_MTB_CONFIG)/liblist.rsp))
	$(info MYP_MTB_LIBRARIES : $(MYP_MTB_LIBRARIES))
	$(info )
	$(eval LIBS += $(filter-out %/main.o,$(MYP_MTB_LIBRARIES)))
	$(info )
	$(eval CFLAGS += -mcpu=cortex-m4 --specs=nano.specs -Og -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -ffunction-sections -fdata-sections -ffat-lto-objects -g -Wall -pipe -DCOMPONENT_4343W -DCOMPONENT_APP_CY8CPROTO_062_4343W -DCOMPONENT_CAT1 -DCOMPONENT_CAT1A -DCOMPONENT_CM0P_SLEEP -DCOMPONENT_CM4 -DCOMPONENT_CM4_0 -DCOMPONENT_Debug -DCOMPONENT_GCC_ARM -DCOMPONENT_HCI_UART -DCOMPONENT_MURATA_1DX -DCOMPONENT_MW_CAT1CM0P -DCOMPONENT_MW_CMSIS -DCOMPONENT_MW_CORE_LIB -DCOMPONENT_MW_CORE_MAKE -DCOMPONENT_MW_MTB_HAL_CAT1 -DCOMPONENT_MW_MTB_PDL_CAT1 -DCOMPONENT_MW_RECIPE_MAKE_CAT1A -DCOMPONENT_MW_RETARGET_IO -DCOMPONENT_PSOC6_02 -DCOMPONENT_SOFTFP -DCOMPONENT_WIFI_INTERFACE_SDIO -DCORE_NAME_CM4_0=1 -DCY8C624ABZI_S2D44 -DCY_APPNAME_mtb_example_hal_hello_world -DCY_SUPPORTS_DEVICE_VALIDATION -DCY_TARGET_BOARD=APP_CY8CPROTO_062_4343W -DCY_USING_HAL -DDEBUG -DTARGET_APP_CY8CPROTO_062_4343W)
	$(info )
	$(eval CXXFLAGS += -mcpu=cortex-m4 --specs=nano.specs -Og -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -ffunction-sections -fdata-sections -ffat-lto-objects -g -Wall -pipe -DCOMPONENT_4343W -DCOMPONENT_APP_CY8CPROTO_062_4343W -DCOMPONENT_CAT1 -DCOMPONENT_CAT1A -DCOMPONENT_CM0P_SLEEP -DCOMPONENT_CM4 -DCOMPONENT_CM4_0 -DCOMPONENT_Debug -DCOMPONENT_GCC_ARM -DCOMPONENT_HCI_UART -DCOMPONENT_MURATA_1DX -DCOMPONENT_MW_CAT1CM0P -DCOMPONENT_MW_CMSIS -DCOMPONENT_MW_CORE_LIB -DCOMPONENT_MW_CORE_MAKE -DCOMPONENT_MW_MTB_HAL_CAT1 -DCOMPONENT_MW_MTB_PDL_CAT1 -DCOMPONENT_MW_RECIPE_MAKE_CAT1A -DCOMPONENT_MW_RETARGET_IO -DCOMPONENT_PSOC6_02 -DCOMPONENT_SOFTFP -DCOMPONENT_WIFI_INTERFACE_SDIO -DCORE_NAME_CM4_0=1 -DCY8C624ABZI_S2D44 -DCY_APPNAME_mtb_example_hal_hello_world -DCY_SUPPORTS_DEVICE_VALIDATION -DCY_TARGET_BOARD=APP_CY8CPROTO_062_4343W -DCY_USING_HAL -DDEBUG -DTARGET_APP_CY8CPROTO_062_4343W)
	$(info )
	$(eval LDFLAGS += -mcpu=cortex-m4 --specs=nano.specs -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -ffunction-sections -fdata-sections -ffat-lto-objects -g -Wall -pipe -Wl,--gc-sections -T$(MTB_APP_DIR)/bsps/TARGET_APP_CY8CPROTO-062-4343W/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/linker.ld -Wl,-Map,$(MTB_APP_DIR)/build/firmware.map $(MYP_MTB_OBJECTS) -Wl,--start-group -Wl,--end-group )
	$(info )
	$(info LD : $(LD))
	$(info )


# -DCY_RETARGET_IO_CONVERT_LF_TO_CRLF

program: $(BUILD)/firmware.hex
	$(info Replacing firmware.hex MTB build folder)
	$(Q)cp $(BUILD)/firmware.hex $(MTB_APP_DIR)/$(MYP_MTB_TARGET)/$(MYP_MTB_CONFIG)/$(MYP_MTB_APPNAME).hex
	$(info Programming using MTB programmer)
	$(Q) cd $(BOARD_DIR); $(MAKE) program

mtb-init:
	$(info Create a mtb-example-hal-hello-world base project for $(BOARD))
	project-creator-cli --board-id $(BOARD) --app-id mtb-example-hal-hello-world --user-app-name $(MTB_APP_DIR_NAME) --target-dir boards/$(BOARD)


.PHONY: program  mtb-init