# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
add_library(containers OBJECT ${SOURCE_FILES})
