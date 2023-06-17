# Copyright (C) 2020 xent
# Project is distributed under the terms of the MIT License

list(APPEND SOURCE_FILES "utils.c")
add_library(xcore_fs OBJECT ${SOURCE_FILES})
target_compile_definitions(xcore_fs PUBLIC -DFS_NAME_LENGTH=${FS_NAME_LENGTH})
