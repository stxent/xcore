# Copyright (C) 2024 xent
# Project is distributed under the terms of the MIT License

string(FIND "${CMAKE_SYSTEM_PROCESSOR}" "cortex" CPU_IS_CORTEX)

if(NOT ${CPU_IS_CORTEX} EQUAL 0)
    add_subdirectory(posix)
else()
    add_subdirectory(generic)
endif()
