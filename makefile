#Copyright (C) 2015 xent
#Project is distributed under the terms of the GNU General Public License v3.0

PROJECT = xcore
PROJECTDIR = $(shell pwd)

CONFIG_FILE ?= .config
CROSS_COMPILE ?= arm-none-eabi-

-include $(CONFIG_FILE)

#Select build type
ifeq ($(CONFIG_TARGET),"X86")
  AR = ar
  CC = gcc
  CXX = g++
  PLATFORM := x86
  CPU_FLAGS := -m32
else ifeq ($(CONFIG_TARGET),"X86-64")
  AR = ar
  CC = gcc
  CXX = g++
  PLATFORM := x86-64
  CPU_FLAGS := -m64
else ifeq ($(CONFIG_TARGET),"CORTEX-M0")
  AR = $(CROSS_COMPILE)ar
  CC = $(CROSS_COMPILE)gcc
  CXX = $(CROSS_COMPILE)g++
  PLATFORM := cortex-m0
  CPU_FLAGS := -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb
  PLATFORM_FLAGS := -specs=redlib.specs -D__REDLIB__
else ifeq ($(CONFIG_TARGET),"CORTEX-M3")
  AR = $(CROSS_COMPILE)ar
  CC = $(CROSS_COMPILE)gcc
  CXX = $(CROSS_COMPILE)g++
  PLATFORM := cortex-m3
  CPU_FLAGS := -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb
  PLATFORM_FLAGS := -specs=redlib.specs -D__REDLIB__
else ifeq ($(CONFIG_TARGET),"CORTEX-M4")
  AR = $(CROSS_COMPILE)ar
  CC = $(CROSS_COMPILE)gcc
  CXX = $(CROSS_COMPILE)g++
  PLATFORM := cortex-m4
  CPU_FLAGS := -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m4 -mthumb
  PLATFORM_FLAGS := -specs=redlib.specs -D__REDLIB__
else
  ifneq ($(MAKECMDGOALS),menuconfig)
    $(error Target architecture is undefined)
  endif
endif

ifeq ($(CONFIG_OPT_LEVEL),"full")
  OPT_FLAGS := -O3
else ifeq ($(CONFIG_OPT_LEVEL),"size")
  OPT_FLAGS := -Os
else ifeq ($(CONFIG_OPT_LEVEL),"none")
  OPT_FLAGS := -O0 -g3
else
  OPT_FLAGS := $(CONFIG_OPT_LEVEL)
endif

#Setup build flags
FLAG_NAMES += CONFIG_CRC_DEBUG CONFIG_UNICODE_DEBUG

define append-flag
  ifeq ($$($(1)),y)
    CONFIG_FLAGS += -D$(1)
  else ifneq ($$($(1)),)
    CONFIG_FLAGS += -D$(1)=$$($(1))
  endif
endef

$(foreach entry,$(FLAG_NAMES),$(eval $(call append-flag,$(entry))))

#Configure common paths and libraries
INCLUDEPATH += -Iinclude
OUTPUTDIR = build_$(PLATFORM)
LDFLAGS += -L$(OUTPUTDIR)
LDLIBS += -l$(PROJECT)

#Configure compiler options
CFLAGS += -std=c11 -Wall -Wcast-qual -Wextra -Winline -pedantic -Wshadow
CFLAGS += $(OPT_FLAGS) $(CPU_FLAGS) $(PLATFORM_FLAGS) $(CONFIG_FLAGS)
CXXFLAGS += -std=c++11 -Wall -Wcast-qual -Wextra -Winline -pedantic -Wshadow -Wold-style-cast
CXXFLAGS += $(OPT_FLAGS) $(CPU_FLAGS) $(PLATFORM_FLAGS) $(CONFIG_FLAGS)

#Search for project modules
LIBRARY_FILE = $(OUTPUTDIR)/lib$(PROJECT).a
TARGETS += $(LIBRARY_FILE)

COBJECTS = $(CSOURCES:%.c=$(OUTPUTDIR)/%.o)
CXXOBJECTS = $(CXXSOURCES:%.cpp=$(OUTPUTDIR)/%.o)

include libxcore/makefile

ifeq ($(CONFIG_EXAMPLES),y)
  include examples/makefile
endif

.PHONY: all clean menuconfig
.SUFFIXES:
.DEFAULT_GOAL = all

all: $(TARGETS)

$(LIBRARY_FILE): $(LIBRARY_OBJECTS)
	$(AR) -r $@ $^

$(OUTPUTDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $(INCLUDEPATH) -MMD -MF $(@:%.o=%.d) -MT $@ $< -o $@

$(OUTPUTDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) $(INCLUDEPATH) -MMD -MF $(@:%.o=%.d) -MT $@ $< -o $@

clean:
	rm -f $(COBJECTS:%.o=%.d) $(COBJECTS) $(CXXOBJECTS:%.o=%.d) $(CXXOBJECTS)
	rm -f $(TARGETS)

menuconfig:
	kconfig-mconf kconfig

ifneq ($(MAKECMDGOALS),clean)
  -include $(COBJECTS:%.o=%.d) $(CXXOBJECTS:%.o=%.d)
endif
