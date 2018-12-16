# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

project(xcore C)
cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0054 NEW)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build: Debug Release." FORCE)
endif()

option(BITWISE_CRC "Enable bitwise implementations of CRC." OFF)
option(BUILD_TESTS "Enable building of example executables." OFF)
option(USE_LTO "Enable Link Time Optimization." OFF)

# Default compiler flags
set(FLAGS_DEFAULT "-fdata-sections -ffunction-sections -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS_DEFAULT} ${FLAGS_CPU}")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

if(${BITWISE_CRC})
    add_definitions("-DCONFIG_FLAG_BITWISE_CRC")
endif()
if(${USE_LTO})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto -ffat-lto-objects")
endif()
if(${BUILD_TESTS})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
endif()

# Gather files for the library
set(TARGET_LIST core generic)
set(LIBRARY_OBJECTS "")

foreach(ENTRY ${TARGET_LIST})
    add_subdirectory(${ENTRY})
    if(TARGET xcore_${ENTRY})
        set(LIBRARY_OBJECTS ${LIBRARY_OBJECTS} $<TARGET_OBJECTS:xcore_${ENTRY}>)
        target_include_directories(xcore_${ENTRY} PRIVATE "${PROJECT_SOURCE_DIR}/include")
    endif()
endforeach()

add_library(${PROJECT_NAME} ${LIBRARY_OBJECTS})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)

if(${BUILD_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
