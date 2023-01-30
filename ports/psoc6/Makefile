BOARD     ?= CY8CPROTO-062-4343W
BOARD_DIR  = boards/$(BOARD)


ifeq ($(wildcard $(BOARD_DIR)/.),)
  $(error Invalid BOARD specified)
endif


# get path to this file
MPY_PATH_TO_MAIN_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
MPY_DIR_OF_MAIN_MAKEFILE  := $(dir $(MPY_PATH_TO_MAIN_MAKEFILE))


# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1

MICROPY_FROZEN_MANIFEST      ?= boards/manifest.py
MICROPY_USER_FROZEN_MANIFEST ?= $(MICROPY_FROZEN_MANIFEST)
FROZEN_MANIFEST              ?= $(MICROPY_FROZEN_MANIFEST)

MICROPY_QSTR_EXTRA_POOL = mp_qstr_frozen_const_pool


CROSS_COMPILE ?= arm-none-eabi-


# include py core make definitions
include ../../py/mkenv.mk
-include mpconfigport.mk
include $(TOP)/py/py.mk
include $(TOP)/extmod/extmod.mk

include $(BOARD_DIR)/mpconfigboard.mk


# get path to this file
MPY_MAIN_BUILD_DIR := $(dir $(MPY_DIR_OF_MAIN_MAKEFILE))/$(BUILD)

$(info MPY_PATH_TO_MAIN_MAKEFILE        : $(MPY_PATH_TO_MAIN_MAKEFILE))
$(info MPY_DIR_OF_MAIN_MAKEFILE         : $(MPY_DIR_OF_MAIN_MAKEFILE))
$(info MPY_MAIN_BUILD_DIR               : $(MPY_MAIN_BUILD_DIR))


INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

LD               = arm-none-eabi-gcc
CFLAGS_CORTEX_M4 = -mthumb -mtune=cortex-m4 -mcpu=cortex-m4 -msoft-float -fsingle-precision-constant -Wdouble-promotion -Wfloat-conversion -UMICROPY_USE_INTERNAL_PRINTF


# c11 : provides "static_assert" (not available in c99) 
# -D_XOPEN_SOURCE=700 : makes sure the setenv/unsetenv headers are included
CFLAGS  = $(INC) -Wall -Werror $(CFLAGS_CORTEX_M4) $(COPT) -std=c11 -D_XOPEN_SOURCE=700 -Wno-error=double-promotion -Wno-error=overflow

#CFLAGS  = $(INC) -Wall -Werror -std=c99 $(CFLAGS_CORTEX_M4) $(COPT)
LDFLAGS = -Wl,--cref -Wl,--gc-sections


# Tune for Debugging or Optimization
ifeq ($(DEBUG), 1)
  CFLAGS         += -O0 -ggdb
  MPY_MTB_CONFIG  = Debug
else
  CFLAGS         += -O3 -Os -DNDEBUG
  CFLAGS         += -fdata-sections -ffunction-sections
  MPY_MTB_CONFIG  = Release
endif


$(info Compiling in $(MPY_MTB_CONFIG) mode !)


# Flags for optional C++ source code
CXXFLAGS += $(filter-out -std=c99,$(CFLAGS))
CXXFLAGS += $(CXXFLAGS_MOD)


# Flags for user C modules
CFLAGS  += $(CFLAGS_MOD)
CFLAGS  += -DFFCONF_H=\"lib/oofatfs/ffconf.h\" -DLFS1_NO_MALLOC -DLFS1_NO_DEBUG -DLFS1_NO_WARN -DLFS1_NO_ERROR -DLFS1_NO_ASSERT -DLFS2_NO_MALLOC -DLFS2_NO_DEBUG -DLFS2_NO_WARN -DLFS2_NO_ERROR -DLFS2_NO_ASSERT

LDFLAGS += $(LDFLAGS_MOD)

LIBS =


MPY_SRC_C += lib/littlefs/lfs1.c lib/littlefs/lfs1_util.c lib/littlefs/lfs2.c lib/littlefs/lfs2_util.c \
		 	 lib/oofatfs/ff.c lib/oofatfs/ffunicode.c \
		 	 \
		 	 shared/readline/readline.c \
			 \
			 shared/runtime/gchelper_native.c \
			 shared/runtime/interrupt_char.c \
		 	 shared/runtime/pyexec.c \
		 	 shared/runtime/stdout_helpers.c \
			 \
		 	 shared/timeutils/timeutils.c \
		     \
			 shared/runtime/sys_stdio_mphal.c 


SRC_ASM = shared/runtime/gchelper_m0.s


SRC_C = \
 	main.c \
	\
	help.c \
	mphalport.c \
	\
    drivers/machine/psoc6_gpio.c \
    drivers/machine/psoc6_i2c.c \
    drivers/machine/psoc6_system.c  \
	\
	modules/gc/modgc.c \
	\
	modules/machine/modmachine.c \
	modules/machine/machine_i2c.c \
	modules/machine/machine_pin.c \
	modules/machine/pins.c \
	modules/machine/machine_rtc.c \
	\
	modules/psoc6/modpsoc6.c \
	modules/psoc6/psoc6_fatfs.c \
	modules/psoc6/psoc6_flash.c \
	modules/psoc6/psoc6_qspi_flash.c \
	\
	modules/time/modutime.c \
	\
	$(MPY_SRC_C) \
    \
	$(BUILD)/frozen_content.c


SRC_C    += $(SRC_USERMOD)
SRC_CXX   = 


SRC_QSTR += $(MPY_SRC_C) $(SRC_USERMOD) $(SRC_EXTMOD_C)
SRC_QSTR += modules/machine/modmachine.c \
            modules/machine/machine_i2c.c \
            modules/machine/machine_pin.c \
			modules/machine/machine_rtc.c \
            \
            modules/os/moduos.c \
			\
            modules/psoc6/modpsoc6.c modules/psoc6/psoc6_fatfs.c modules/psoc6/psoc6_flash.c modules/psoc6/psoc6_qspi_flash.c \
			\
			modules/time/modutime.c


OBJ += $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o)) $(addprefix $(BUILD)/, $(SRC_ASM:.s=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CXX:.cpp=.o))


# switch for debug mode, also added to mpconfigport.h
# TODO: keep the more suitable one, delete the other
MP_LOGGER_DEBUG ?= 0

ifeq ($(MP_LOGGER_DEBUG), 1)
  CFLAGS += -DMICROPY_LOGGER_DEBUG=1
endif


$(MPY_MAIN_BUILD_DIR)/firmware.elf: mpy_define_mtb_vars $(OBJ) $(MPY_MTB_LIBRARIES) $(LIBS)
	$(info )
	$(info Linking $@ ...)
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(info Linking $@ done.)
	$(Q)$(SIZE) $@ -A
	$(info )


$(MPY_MAIN_BUILD_DIR)/firmware.hex: $(MPY_MAIN_BUILD_DIR)/firmware.elf
	$(Q)$(OBJCOPY) -O ihex $^ $@


# include adapter makefile
include $(BOARD_DIR)/makefile_mtb.mk

# include py core make definitions
include $(TOP)/py/mkrules.mk


all: $(MPY_MAIN_BUILD_DIR)/firmware.hex $(MPY_PATH_TO_MAIN_MAKEFILE)

rebuild: clean mpy_mtb_clean all
