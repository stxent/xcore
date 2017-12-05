# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

project(xcore C)
cmake_minimum_required(VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build: Debug Release." FORCE)
endif()

option(BUILD_TESTS "Enable building of example executables." OFF)
option(USE_LTO "Enable Link Time Optimization." OFF)

# Default compiler flags
set(FLAGS_DEFAULT "-fdata-sections -ffunction-sections -fmessage-length=0 -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS_DEFAULT} ${FLAGS_CPU}")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

if(${USE_LTO})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto -ffat-lto-objects")
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")

# Gather files for the library
add_subdirectory(containers)
add_subdirectory(core)
add_subdirectory(crc)
add_subdirectory(generic)

set(LIBRARY_OBJECTS $<TARGET_OBJECTS:containers> $<TARGET_OBJECTS:crc> $<TARGET_OBJECTS:generic>)
if(TARGET core)
    set(LIBRARY_OBJECTS ${LIBRARY_OBJECTS} $<TARGET_OBJECTS:core>)
endif()

add_library(${PROJECT_NAME} ${LIBRARY_OBJECTS})
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)

if(${BUILD_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
