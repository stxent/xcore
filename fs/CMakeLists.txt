# Copyright (C) 2020 xent
# Project is distributed under the terms of the MIT License

file(GLOB_RECURSE SOURCE_FILES "*.c")
add_library(xcore_fs OBJECT ${SOURCE_FILES})
target_compile_definitions(xcore_fs PRIVATE -DFS_NAME_LENGTH=${FS_NAME_LENGTH})
