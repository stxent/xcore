# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

string(FIND "${CMAKE_SYSTEM_PROCESSOR}" "cortex" CPU_IS_CORTEX)

if(${CPU_IS_CORTEX} EQUAL 0)
    add_subdirectory(cortex)
endif()
