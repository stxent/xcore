#Copyright (C) 2015 xent
#Project is distributed under the terms of the GNU General Public License v3.0

CONTAINER_SOURCES := $(shell find libxcore/containers -name *.c)

LIBRARY_SOURCES += $(CONTAINER_SOURCES)
LIBRARY_SOURCES += libxcore/entity.c
LIBRARY_SOURCES += libxcore/unicode.c

ifeq ($(CONFIG_CRC_CRC7),y)
  LIBRARY_SOURCES += libxcore/crc/crc7.c
endif

ifeq ($(CONFIG_CRC_CRC8DALLAS),y)
  LIBRARY_SOURCES += libxcore/crc/crc8dallas.c
endif

ifeq ($(CONFIG_CRC_CRC16),y)
  LIBRARY_SOURCES += libxcore/crc/crc16.c
endif

LIBRARY_OBJECTS = $(LIBRARY_SOURCES:%.c=$(OUTPUTDIR)/%.o)
CSOURCES += $(LIBRARY_SOURCES)

#Setup build flags
FLAG_NAMES += CONFIG_CRC7_BITWISE CONFIG_CRC8DALLAS_BITWISE CONFIG_CRC16_BITWISE

define append-flag
  ifeq ($$($(1)),y)
    CONFIG_FLAGS += -D$(1)
  else ifneq ($$($(1)),)
    CONFIG_FLAGS += -D$(1)=$$($(1))
  endif
endef

$(foreach entry,$(FLAG_NAMES),$(eval $(call append-flag,$(entry))))
