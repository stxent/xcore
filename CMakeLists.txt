#Copyright (C) 2017 xent
#Project is distributed under the terms of the GNU General Public License v3.0

project(xcore C)
cmake_minimum_required(VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build: Debug Release." FORCE)
endif()

option(BUILD_TESTS "Enable building of example executables." OFF)
option(USE_LTO "Enable Link Time Optimization." OFF)

#Default compiler flags
set(FLAGS_DEFAULT "-fdata-sections -ffunction-sections -fmessage-length=0 -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS_DEFAULT} ${FLAGS_CPU}")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

if(${USE_LTO})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto -ffat-lto-objects")
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")

#Gather files for the library
include("${PROJECT_SOURCE_DIR}/libxcore/CMakeLists.txt")

add_library(${PROJECT_NAME} ${LIBRARY_SOURCES} ${LIBRARY_SOURCES_CORE})
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)

if(${BUILD_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
