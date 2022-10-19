BOARD ?= CY8CPROTO-062-4343W
BOARD_DIR = boards/$(BOARD)

ifeq ($(wildcard $(BOARD_DIR)/.),)
$(error Invalid BOARD specified)
endif

include ../../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE ?= arm-none-eabi-

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

LD               = $(CROSS_COMPILE)gcc
CFLAGS_CORTEX_M4 = -mthumb -mtune=cortex-m4 -mcpu=cortex-m4 -msoft-float -fsingle-precision-constant -Wdouble-promotion -Wfloat-conversion -UMICROPY_USE_INTERNAL_PRINTF

# original code had additional option as compared to settings below :
#CFLAGS += -nostdlib
#LDFLAGS = -nostdlib


# use for HelloWorld MTB
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

#build/main.o: main.c
#	/home/jensb/ModusToolbox/tools_3.0/gcc/bin/arm-none-eabi-gcc -c $(CFLAGS) -o $@ $< $(LIBS)  

SRC_C = \
 	main.c \
	uart_core.c \
	shared/readline/readline.c \
	shared/runtime/pyexec.c \
	shared/runtime/stdout_helpers.c \
	$(BUILD)/_frozen_mpy.c
#	shared/libc/printf.c # including this prevents fw to execute !


SRC_C += shared/libc/string0.c

SRC_C   += $(SRC_MOD)
SRC_CXX += $(SRC_MOD_CXX)

SRC_QSTR += shared/readline/readline.c shared/runtime/pyexec.c
SRC_QSTR += $(SRC_MOD) $(SRC_MOD_CXX)

OBJ += $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CXX:.cpp=.o))


all: $(BUILD)/firmware.elf


$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@


$(OBJ): myp_define_mtb_vars


$(BUILD)/firmware.elf: $(OBJ)
	$(info "Linking $@ ...   $(OBJ)")
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ 
# $(LIBS)  
#	/cygdrive/c/Users/bargfred/ModusToolbox/tools_3.0/gcc/bin/arm-none-eabi-g++ $(LDFLAGS) -o $@ $(OBJ) $(LIBS)  
#	/home/jensb/ModusToolbox/tools_3.0/gcc/bin/arm-none-eabi-g++ $(LDFLAGS) -o $@ $^ $(LIBS)  
	$(Q)$(SIZE) $@

$(BUILD)/firmware.hex: $(BUILD)/firmware.elf
	$(Q)$(OBJCOPY) -O ihex $^ $@

#	$(Q)$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $^ $(BUILD)/firmware.bin
# $(Q)$(OBJCOPY) -O ihex -j .isr_vector -j .text -j .data $^ $@

include $(TOP)/py/mkrules.mk

# TODO: get path to board from variables/command line option !
include makefile_mtb.mk

