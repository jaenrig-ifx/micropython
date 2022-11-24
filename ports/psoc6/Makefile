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

CROSS_COMPILE ?= arm-none-eabi-


# include py core make definitions
include ../../py/mkenv.mk
-include mpconfigport.mk
include $(TOP)/py/py.mk


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


# used for HelloWorld MTB
CFLAGS  = $(INC) -Wall -Werror -std=c99 $(CFLAGS_CORTEX_M4) $(COPT)
LDFLAGS = -Wl,--cref -Wl,--gc-sections


# Tune for Debugging or Optimization
ifeq ($(DEBUG), 1)
  CFLAGS += -O0 -ggdb
else
  CFLAGS += -Os -DNDEBUG
  CFLAGS += -fdata-sections -ffunction-sections
endif


# Flags for optional C++ source code
CXXFLAGS += $(filter-out -std=c99,$(CFLAGS))
CXXFLAGS += $(CXXFLAGS_MOD)


# Flags for user C modules
CFLAGS  += $(CFLAGS_MOD)
CFLAGS  += -DFFCONF_H=\"lib/oofatfs/ffconf.h\" -DLFS1_NO_MALLOC -DLFS1_NO_DEBUG -DLFS1_NO_WARN -DLFS1_NO_ERROR -DLFS1_NO_ASSERT -DLFS2_NO_MALLOC -DLFS2_NO_DEBUG -DLFS2_NO_WARN -DLFS2_NO_ERROR -DLFS2_NO_ASSERT

LDFLAGS += $(LDFLAGS_MOD)

LIBS =


MPY_SRC_C += extmod/vfs_blockdev.c extmod/vfs.c \
			 extmod/vfs_lfs.c \
		 	 extmod/vfs_fat.c extmod/vfs_fat_diskio.c extmod/vfs_fat_file.c \
		 	 extmod/moduos.c \
		 	 \
		 	 lib/littlefs/lfs1.c lib/littlefs/lfs1_util.c lib/littlefs/lfs2.c lib/littlefs/lfs2_util.c \
		 	 lib/oofatfs/ff.c lib/oofatfs/ffunicode.c \
		 	 \
		 	 py/modcmath.c py/moduerrno.c py/repl.c \
		 	 \
		 	 shared/runtime/pyexec.c \
		 	 shared/readline/readline.c \
		 	 shared/runtime/stdout_helpers.c \
		 	 shared/libc/string0.c \
		 	 shared/timeutils/timeutils.c


SRC_C_FROZEN = $(BUILD)/_frozentest_mpy.c


SRC_C = \
 	main.c \
	fatfs_port.c \
	psoc6_flash.c \
	uart_core.c \
	\
	modules/machine/modmachine.c \
	modules/machine/pin.c \
	\
	$(SRC_C_FROZEN) \
	\
	$(MPY_SRC_C)


SRC_C    += $(SRC_MOD) $(SRC_USERMOD) 
SRC_CXX  += $(SRC_MOD_CXX)


SRC_QSTR += $(MPY_SRC_C) $(SRC_MOD) $(SRC_MOD_CXX) $(SRC_USERMOD) 
SRC_QSTR += modules/machine/modmachine.c modules/machine/pin.c


OBJ += $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CXX:.cpp=.o))


all: $(MPY_MAIN_BUILD_DIR)/firmware.hex


freeze/frozentest.mpy: $(BUILD)/genhdr/qstrdefs.generated.h
	env PATH="../../mpy-cross/build:${PATH}" ../../tools/mpy_cross_all.py freeze


$(SRC_C_FROZEN): freeze/frozentest.mpy
	$(info )
	$(info Freezing bytecode ...)
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@
	$(info Freezing bytecode done.)



$(MPY_MAIN_BUILD_DIR)/firmware.elf: mpy_define_mtb_vars $(OBJ)
	$(info )
	$(info Linking $@ ...)
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ 
	$(info Linking $@ done.)
	$(Q)$(SIZE) $@


$(MPY_MAIN_BUILD_DIR)/firmware.hex: $(MPY_MAIN_BUILD_DIR)/firmware.elf
	$(Q)$(OBJCOPY) -O ihex $^ $@


# include adapter makefile
include $(BOARD_DIR)/makefile_mtb.mk

# include py core make definitions
include $(TOP)/py/mkrules.mk
