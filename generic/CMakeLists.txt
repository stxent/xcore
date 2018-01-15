# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

file(GLOB_RECURSE SOURCE_FILES "*.c")
add_library(generic OBJECT ${SOURCE_FILES})
