#Copyright (C) 2015 xent
#Project is distributed under the terms of the GNU General Public License v3.0

PROJECT_FLAGS += CONFIG_CRC7_BITWISE
PROJECT_FLAGS += CONFIG_CRC8_DALLAS_BITWISE
PROJECT_FLAGS += CONFIG_CRC16_CCITT_BITWISE
PROJECT_FLAGS += CONFIG_CRC32_BITWISE

CONTAINER_SOURCES := $(shell find libxcore/containers -name *.c)

LIBRARY_SOURCES += $(CONTAINER_SOURCES)
LIBRARY_SOURCES += libxcore/entity.c
LIBRARY_SOURCES += libxcore/realtime.c
LIBRARY_SOURCES += libxcore/unicode.c

ifneq ($(findstring cortex,$(PLATFORM)),)
  LIBRARY_SOURCES_CORE := $(shell find libxcore/core/$(subst -,/,$(PLATFORM)) -name *.c)
  LIBRARY_SOURCES += $(LIBRARY_SOURCES_CORE)
endif

ifeq ($(CONFIG_CRC_CRC7),y)
  LIBRARY_SOURCES += libxcore/crc/crc7.c
endif

ifeq ($(CONFIG_CRC_CRC8_DALLAS),y)
  LIBRARY_SOURCES += libxcore/crc/crc8_dallas.c
endif

ifeq ($(CONFIG_CRC_CRC16_CCITT),y)
  LIBRARY_SOURCES += libxcore/crc/crc16_ccitt.c
endif

ifeq ($(CONFIG_CRC_CRC32),y)
  LIBRARY_SOURCES += libxcore/crc/crc32.c
endif

LIBRARY_OBJECTS = $(LIBRARY_SOURCES:%.c=$(OUTPUT_DIR)/%.o)
CSOURCES += $(LIBRARY_SOURCES)

LIBRARY_FILE = $(OUTPUT_DIR)/lib$(PROJECT).a
TARGETS += $(LIBRARY_FILE)

$(LIBRARY_FILE): $(LIBRARY_OBJECTS)
	$(AR) -r $@ $^
