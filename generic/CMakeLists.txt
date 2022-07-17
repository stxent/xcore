# Copyright (C) 2017 xent
# Project is distributed under the terms of the MIT License

include(CheckIncludeFile)
CHECK_INCLUDE_FILE(uchar.h UCHAR_FOUND)

list(APPEND SOURCE_FILES "entity.c")
list(APPEND SOURCE_FILES "realtime.c")
list(APPEND SOURCE_FILES "unicode.c")

# Container directory
list(APPEND SOURCE_FILES "containers/array.c")
list(APPEND SOURCE_FILES "containers/byte_queue.c")
list(APPEND SOURCE_FILES "containers/list.c")
list(APPEND SOURCE_FILES "containers/queue.c")
list(APPEND SOURCE_FILES "containers/tg_list.c")

# Checksum directory
list(APPEND SOURCE_FILES "crc/crc7.c")
list(APPEND SOURCE_FILES "crc/crc8_dallas.c")
list(APPEND SOURCE_FILES "crc/crc16_ccitt.c")
list(APPEND SOURCE_FILES "crc/crc32.c")

add_library(xcore_generic OBJECT ${SOURCE_FILES})

if(NOT UCHAR_FOUND)
    configure_file("${PROJECT_SOURCE_DIR}/stubs/uchar.h" "${PROJECT_BINARY_DIR}/include/uchar.h" COPYONLY)
    set_target_properties(xcore_generic PROPERTIES
            PUBLIC_HEADER "${PROJECT_BINARY_DIR}/include/uchar.h"
    )
    target_include_directories(xcore_generic PUBLIC
            $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
endif()

if(BITWISE_CRC)
    target_compile_definitions(xcore_generic PRIVATE -DCONFIG_FLAG_BITWISE_CRC)
endif()
