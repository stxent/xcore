# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

include(FindPkgConfig)
pkg_check_modules(CHECK REQUIRED check)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(ListDirectories)
list_directories(TEST_LIST "${CMAKE_CURRENT_SOURCE_DIR}")

foreach(TEST_NAME ${TEST_LIST})
    file(GLOB_RECURSE TEST_SOURCES "${TEST_NAME}/*.c")
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    add_test(${TEST_NAME} ${TEST_NAME})
    target_compile_definitions(${TEST_NAME} PRIVATE _POSIX_C_SOURCE=200809L)
    target_compile_options(${TEST_NAME} PRIVATE --coverage)
    target_link_libraries(${TEST_NAME} PRIVATE xcore check gcov)
endforeach()
