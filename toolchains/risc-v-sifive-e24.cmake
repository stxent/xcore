# Copyright (C) 2025 xent
# Project is distributed under the terms of the MIT License

cmake_minimum_required(VERSION 3.21)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "risc-v-sifive-e24")

set(CMAKE_ASM_COMPILER "riscv-none-elf-gcc")
set(CMAKE_C_COMPILER "riscv-none-elf-gcc")
set(CMAKE_CXX_COMPILER "riscv-none-elf-g++")
set(CMAKE_SIZE "riscv-none-elf-size")

set(FLAGS_CPU "-nostartfiles -march=rv32imac_zicsr -mtune=sifive-e24 -mabi=ilp32 -mcmodel=medany")

# Disable linking stage because cross-compiling toolchain cannot link without custom linker script
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
