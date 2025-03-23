# Copyright (C) 2024 xent
# Project is distributed under the terms of the MIT License

cmake_minimum_required(VERSION 3.21)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "cortex-m7")

set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_SIZE "arm-none-eabi-size")

set(FLAGS_CPU "-nostartfiles -mthumb -march=armv7e-m -mtune=cortex-m7")

# Disable linking stage because cross-compiling toolchain cannot link without custom linker script
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
