# Copyright (C) 2024 xent
# Project is distributed under the terms of the MIT License

find_package(Threads REQUIRED)

list(APPEND SOURCE_FILES "mutex.c")
list(APPEND SOURCE_FILES "semaphore.c")
list(APPEND SOURCE_FILES "thread.c")

add_library(xcore_os OBJECT ${SOURCE_FILES})
target_link_libraries(xcore_os PUBLIC Threads::Threads)
