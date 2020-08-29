# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

project(xcore C)
cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0069 NEW)

option(BITWISE_CRC "Enable bitwise implementations of CRC." OFF)
option(BUILD_TESTS "Enable building of example executables." OFF)
option(USE_LTO "Enable Link Time Optimization." OFF)

# Default compiler flags
set(FLAGS_DEFAULT "-fdata-sections -ffunction-sections -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS_DEFAULT} ${FLAGS_CPU}")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

if(${BUILD_TESTS})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
endif()

if(${USE_LTO})
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_COMPILE_OPTIONS_IPO -flto -ffat-lto-objects)
    endif()
endif()

# Gather files for the library
set(DIRECTORY_LIST core generic)
set(LIBRARY_OBJECTS "")
set(LIBRARY_TARGETS "")

foreach(ENTRY ${DIRECTORY_LIST})
    add_subdirectory(${ENTRY})
    if(TARGET xcore_${ENTRY})
        list(APPEND LIBRARY_TARGETS xcore_${ENTRY})
        list(APPEND LIBRARY_OBJECTS $<TARGET_OBJECTS:xcore_${ENTRY}>)
        target_include_directories(xcore_${ENTRY} PUBLIC "${PROJECT_SOURCE_DIR}/include")
    endif()
endforeach()

add_library(${PROJECT_NAME} ${LIBRARY_OBJECTS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_TARGETS})

install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)

if(${BUILD_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
