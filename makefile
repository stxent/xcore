#Copyright (C) 2015 xent
#Project is distributed under the terms of the GNU General Public License v3.0

PROJECT := xcore
PROJECTDIR := $(shell pwd)

CONFIG_FILE ?= .config
CROSS_COMPILE ?= arm-none-eabi-

-include $(CONFIG_FILE)
OPTION_NAMES += PLATFORM

#Process configuration options
define process-option
  $(1) := $$(CONFIG_$(1):"%"=%)
endef

$(foreach entry,$(OPTION_NAMES),$(eval $(call process-option,$(entry))))

#Select build type
ifneq ($(findstring x86,$(PLATFORM)),)
  AR := ar
  CC := gcc
  CXX := g++
  ifeq ($(CONFIG_TARGET),"x86")
    CPU_FLAGS += -m32
  else
    CPU_FLAGS += -m64
  endif
else ifneq ($(findstring cortex-m,$(PLATFORM)),)
  AR := $(CROSS_COMPILE)ar
  CC := $(CROSS_COMPILE)gcc
  CXX := $(CROSS_COMPILE)g++
  CPU_FLAGS += -mcpu=$(PLATFORM) -mthumb
  CPU_FLAGS += -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections
else ifneq ($(MAKECMDGOALS),menuconfig)
  $(error Target architecture is undefined)
endif

ifeq ($(CONFIG_OPTIMIZATIONS),"full")
  OPT_FLAGS := -O3 -DNDEBUG
else ifeq ($(CONFIG_OPTIMIZATIONS),"size")
  OPT_FLAGS := -Os -DNDEBUG
else ifeq ($(CONFIG_OPTIMIZATIONS),"none")
  OPT_FLAGS := -O0 -g3
else
  OPT_FLAGS := $(CONFIG_OPTIMIZATIONS)
endif

#Configure common paths and libraries
INCLUDEPATH += -Iinclude
OUTPUTDIR = build_$(PLATFORM)
LDFLAGS += -L$(OUTPUTDIR)
LDLIBS += -l$(PROJECT)

#Configure compiler options
CFLAGS += -std=c11 -Wall -Wextra -Winline -pedantic -Wshadow -Wcast-qual
CFLAGS += $(OPT_FLAGS) $(CPU_FLAGS) $(CONFIG_FLAGS)
CXXFLAGS += -std=c++11 -Wall -Wextra -Winline -pedantic -Wshadow -Wold-style-cast -Wcast-qual
CXXFLAGS += -fno-exceptions -fno-rtti
CXXFLAGS += $(OPT_FLAGS) $(CPU_FLAGS) $(CONFIG_FLAGS)

#Other makefiles of the project
include lib$(PROJECT)/makefile
include examples/makefile

#Process project options
define append-flag
  ifeq ($$($(1)),y)
    CONFIG_FLAGS += -D$(1)
  else ifneq ($$($(1)),)
    CONFIG_FLAGS += -D$(1)=$$($(1))
  endif
endef

$(foreach entry,$(FLAG_NAMES),$(eval $(call append-flag,$(entry))))

COBJECTS = $(CSOURCES:%.c=$(OUTPUTDIR)/%.o)
CXXOBJECTS = $(CXXSOURCES:%.cpp=$(OUTPUTDIR)/%.o)

#Define default targets
.PHONY: all clean menuconfig
.SUFFIXES:
.DEFAULT_GOAL = all

all: $(TARGETS)

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
