# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

file(GLOB_RECURSE SOURCE_FILES "*.c")
add_library(xcore_generic OBJECT ${SOURCE_FILES})

if(${BITWISE_CRC})
    target_compile_definitions(xcore_generic PRIVATE -DCONFIG_FLAG_BITWISE_CRC)
endif()
