# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

include(CheckIncludeFile)
CHECK_INCLUDE_FILE(uchar.h UCHAR_FOUND)

file(GLOB_RECURSE SOURCE_FILES "*.c")
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

if(${BITWISE_CRC})
    target_compile_definitions(xcore_generic PRIVATE -DCONFIG_FLAG_BITWISE_CRC)
endif()
