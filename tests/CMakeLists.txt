#Copyright (C) 2017 xent
#Project is distributed under the terms of the GNU General Public License v3.0

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_POSIX_C_SOURCE=200809L")

macro(list_directories SEARCH_RESULT DIRECTORY_PATH)
    file(GLOB ENTRY_LIST RELATIVE ${DIRECTORY_PATH} ${DIRECTORY_PATH}/*)
    set(DIRECTORY_LIST "")
    foreach(ENTRY ${ENTRY_LIST})
        if(IS_DIRECTORY ${DIRECTORY_PATH}/${ENTRY})
            list(APPEND DIRECTORY_LIST ${ENTRY})
        endif()
    endforeach()
    set(${SEARCH_RESULT} ${DIRECTORY_LIST})
endmacro()

list_directories(EXAMPLES_LIST "${CMAKE_CURRENT_SOURCE_DIR}")

foreach(EXAMPLE_NAME ${EXAMPLES_LIST})
    file(GLOB_RECURSE EXAMPLE_SOURCES "${EXAMPLE_NAME}/*.c")
    add_executable("${EXAMPLE_NAME}" ${EXAMPLE_SOURCES})
    target_link_libraries("${EXAMPLE_NAME}" xcore)
    add_test("${EXAMPLE_NAME}" "${EXAMPLE_NAME}")
endforeach()
