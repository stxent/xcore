# Copyright (C) 2017 xent
# Project is distributed under the terms of the GNU General Public License v3.0

include(FindPkgConfig)
pkg_check_modules(CHECK REQUIRED check)

function(list_directories _search_results _directory_path)
    file(GLOB _entry_list RELATIVE "${_directory_path}" "${_directory_path}/*")
    set(_directory_list "")
    foreach(_entry ${_entry_list})
        if(IS_DIRECTORY "${_directory_path}/${_entry}")
            list(APPEND _directory_list "${_entry}")
        endif()
    endforeach()
    set(${_search_results} ${_directory_list} PARENT_SCOPE)
endfunction()

list_directories(EXAMPLES_LIST "${CMAKE_CURRENT_SOURCE_DIR}")

foreach(EXAMPLE_NAME ${EXAMPLES_LIST})
    file(GLOB_RECURSE EXAMPLE_SOURCES "${EXAMPLE_NAME}/*.c")
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})
    add_test(${EXAMPLE_NAME} ${EXAMPLE_NAME})
    target_compile_definitions(${EXAMPLE_NAME} PRIVATE _POSIX_C_SOURCE=200809L)
    target_compile_options(${EXAMPLE_NAME} PRIVATE --coverage)
    target_link_libraries(${EXAMPLE_NAME} xcore check gcov)
endforeach()
