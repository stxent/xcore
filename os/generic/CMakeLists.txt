# Copyright (C) 2024 xent
# Project is distributed under the terms of the MIT License

list(APPEND SOURCE_FILES "mutex.c")
list(APPEND SOURCE_FILES "semaphore.c")
add_library(xcore_os OBJECT ${SOURCE_FILES})
