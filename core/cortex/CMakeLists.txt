# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

string(REPLACE "cortex-" "" CPU_CORE "${CMAKE_SYSTEM_PROCESSOR}")
file(GLOB SOURCE_FILES "${CPU_CORE}/*.c")
add_library(core OBJECT ${SOURCE_FILES})
