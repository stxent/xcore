# Copyright (C) 2017 xent
# Project is distributed under the terms of the MIT License

cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0069 NEW)
project(xcore C)

option(BITWISE_CRC "Enable bitwise implementations of CRC." OFF)
option(BUILD_TESTING "Enable testing support." OFF)
option(USE_LTO "Enable Link Time Optimization." OFF)
set(FS_NAME_LENGTH 64 CACHE STRING "Maximum length of filesystem entry name in bytes.")

# Default compiler flags

set(FLAGS_DEFAULT "-fdata-sections -ffunction-sections")
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_INIT} ${FLAGS_DEFAULT} ${FLAGS_CPU}")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

if(USE_LTO)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_COMPILE_OPTIONS_IPO -flto -ffat-lto-objects)
    endif()
endif()

# Collect files for object libraries

set(DIRECTORY_LIST core fs generic)
set(LIBRARY_OBJECTS "")
set(LIBRARY_TARGETS "")

foreach(ENTRY ${DIRECTORY_LIST})
    add_subdirectory(${ENTRY})
    if(TARGET xcore_${ENTRY})
        list(APPEND LIBRARY_TARGETS xcore_${ENTRY})
        list(APPEND LIBRARY_OBJECTS $<TARGET_OBJECTS:xcore_${ENTRY}>)

        target_include_directories(xcore_${ENTRY} PUBLIC
                $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>
        )

        if(BUILD_TESTING)
            target_compile_options(xcore_${ENTRY} PRIVATE --coverage)
        endif()

        if(UNIX)
            target_compile_definitions(xcore_${ENTRY} PUBLIC _POSIX_C_SOURCE=200809L)
        endif()

        install(TARGETS xcore_${ENTRY}
                EXPORT xcore-targets
                PUBLIC_HEADER DESTINATION include
        )
    endif()
endforeach()

add_library(${PROJECT_NAME} ${LIBRARY_OBJECTS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_TARGETS})

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Configure library installation

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/xcore
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/toolchains
        DESTINATION lib/xcore
        FILES_MATCHING PATTERN "*.cmake"
)
install(TARGETS ${PROJECT_NAME}
        EXPORT xcore-targets
        ARCHIVE DESTINATION lib
)
install(EXPORT xcore-targets
        FILE xcore-targets.cmake
        DESTINATION lib/xcore
)
install(FILES "${PROJECT_SOURCE_DIR}/config.cmake.in"
        DESTINATION lib/xcore
        RENAME xcore-config.cmake
)
