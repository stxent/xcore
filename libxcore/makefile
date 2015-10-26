#Copyright (C) 2015 xent
#Project is distributed under the terms of the GNU General Public License v3.0

PROJECT_FLAGS += CONFIG_CRC7_BITWISE CONFIG_CRC8DALLAS_BITWISE CONFIG_CRC16_BITWISE

CONTAINER_SOURCES := $(shell find libxcore/containers -name *.c)

LIBRARY_SOURCES += $(CONTAINER_SOURCES)
LIBRARY_SOURCES += libxcore/entity.c
LIBRARY_SOURCES += libxcore/realtime.c
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

LIBRARY_OBJECTS = $(LIBRARY_SOURCES:%.c=$(OUTPUT_DIR)/%.o)
CSOURCES += $(LIBRARY_SOURCES)

LIBRARY_FILE = $(OUTPUT_DIR)/lib$(PROJECT).a
TARGETS += $(LIBRARY_FILE)

$(LIBRARY_FILE): $(OPTION_FILE) $(LIBRARY_OBJECTS)
	$(AR) -r $@ $(filter-out $(OPTION_FILE),$^)
