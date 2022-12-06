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
include $(TOP)/py/py.mk


# get path to this file
MPY_MAIN_BUILD_DIR := $(dir $(MPY_DIR_OF_MAIN_MAKEFILE))/$(BUILD)

$(info MPY_PATH_TO_MAIN_MAKEFILE        : $(MPY_PATH_TO_MAIN_MAKEFILE))
$(info MPY_DIR_OF_MAIN_MAKEFILE         : $(MPY_DIR_OF_MAIN_MAKEFILE))
$(info MPY_MAIN_BUILD_DIR               : $(MPY_MAIN_BUILD_DIR))


INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

LD               = ~/ModusToolbox/tools_3.0/gcc/bin/arm-none-eabi-gcc
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
LDFLAGS += $(LDFLAGS_MOD)

LIBS =

SRC_C = \
 	main.c \
	uart_core.c \
	mplogger.c \
	\
	modules/machine/modmachine.c \
	modules/machine/machine_pin.c \
	modules/machine/pins.c \
	\
	shared/runtime/pyexec.c \
	shared/readline/readline.c \
	shared/runtime/stdout_helpers.c \
	\
	drivers/psoc6_system.c  \
	drivers/psoc6_gpio.c \
	\
	mphalport.c \
	$(BUILD)/_frozen_mpy.c

# the order of driver sources matter above. 
# TODO: investigate the circular includes and remove the redundant ones

SRC_C    += shared/libc/string0.c

SRC_C    += $(SRC_MOD) $(SRC_USERMOD) 
SRC_CXX  += $(SRC_MOD_CXX)

SRC_QSTR += shared/readline/readline.c shared/runtime/pyexec.c
SRC_QSTR += $(SRC_MOD) $(SRC_MOD_CXX) $(SRC_USERMOD) 
SRC_QSTR += modules/machine/modmachine.c modules/machine/machine_pin.c


OBJ += $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CXX:.cpp=.o))

# switch for debug mode, also added to mpconfigport.h
# TODO: keep the more suitable one, delete the other
MP_LOGGER_DEBUG ?= 0

ifeq ($(MP_LOGGER_DEBUG), 1)
  CFLAGS += -DMICROPY_LOGGER_DEBUG=1
endif

all: $(MPY_MAIN_BUILD_DIR)/firmware.hex


$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(info )
	$(info Freezing bytecode)
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@


$(MPY_MAIN_BUILD_DIR)/firmware.elf: mpy_define_mtb_vars $(OBJ)
	$(info )
	$(info Linking $@ ...)
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ 
	$(Q)$(SIZE) $@


$(MPY_MAIN_BUILD_DIR)/firmware.hex: $(MPY_MAIN_BUILD_DIR)/firmware.elf
	$(Q)$(OBJCOPY) -O ihex $^ $@


# include adapter makefile
include $(BOARD_DIR)/makefile_mtb.mk

# include py core make definitions
include $(TOP)/py/mkrules.mk
