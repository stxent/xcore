#Copyright (C) 2017 xent
#Project is distributed under the terms of the GNU General Public License v3.0

string(FIND "${CMAKE_SYSTEM_PROCESSOR}" "cortex" CPU_IS_CORTEX)

if(${CPU_IS_CORTEX} EQUAL 0)
    string(REPLACE "cortex-" "" CPU_CORE "${CMAKE_SYSTEM_PROCESSOR}")
    file(GLOB_RECURSE LIBRARY_SOURCES_CORE "${PROJECT_SOURCE_DIR}/libxcore/core/cortex/${CPU_CORE}/*.c")
endif()
