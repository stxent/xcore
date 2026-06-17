# Xcore

![C23](https://img.shields.io/badge/C-23-blue)
![CMake](https://img.shields.io/badge/CMake-3.21+-blue)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A lightweight C library for embedded systems containing basic interfaces,
algorithms, and containers. It is written using the C23 standard and CMake.
CTest is used for testing.

## Supported Platforms

Xcore supports the following platforms:

* **ARM Cortex‑M series**:
  * Cortex‑M0
  * Cortex‑M0+
  * Cortex‑M3
  * Cortex‑M4
  * Cortex‑M7
* **RISC‑V RV32**
* **x86**

## Required Packages

To build and use Xcore, you need the following packages:

* **GCC 13 or newer** — the GNU Compiler Collection, required for
  building the x86 version and running unit tests.
* **ARM GCC 13 or newer** — Arm GNU Toolchain, required for
  Cortex-M embedded targets.
* **RISC-V GCC 13 or newer** — RISC-V GNU Toolchain, required for
  RISC-V embedded targets.
* **CMake 3.21 or newer** — used for configuring and generating build
  systems across platforms.
* **libcheck** — required for building and running unit tests
  (optional if tests are disabled).

## Library Contents

### Header Files

Platform‑specific header files provide access to hardware‑accelerated
computation functions and architecture‑optimized routines. These allow
developers to take advantage of low‑level capabilities when available.

### Containers

A selection of containers with different allocation strategies:

* **Statically allocated** (memory is pre‑reserved, no runtime allocation):
  * Array — fixed‑size sequential container
  * AVL Tree — self‑balancing binary search tree
  * Queue — FIFO structure
  * Byte Queue — optimized for byte stream handling
* **Dynamically allocated**:
  * List — linked list with dynamic memory management
* **Type-generic statically allocated**:
  * TG Array — generic fixed‑size array
  * TG Queue — generic FIFO
* **Type-generic dynamically allocated**:
  * TG List — generic linked list

### Basic Interfaces

Core abstractions for building robust applications:

* **OOP implementation in C** — object‑oriented patterns using structs
  and function pointers
* **Generic I/O interface** — for byte‑stream devices (e.g., UART, SPI)
* **Generic I/O stream** — packet‑based communication (e.g., Ethernet)
* **Date and time handling** — utilities for timestamp operations
* **Unicode support** — basic UTF‑8/UTF‑16 handling and string utilities
* **OS primitives** — abstractions for threading, mutexes, and signaling
  (where available)
* **File system interface** — portable access to files and directories
  with common operations
* **CRC implementations** — multiple variants with fixed polynomials

## Usage Examples

Here are practical examples to help you integrate Xcore into your projects.

1. Including in a CMake Project

To add Xcore as a subdirectory in your CMake project, use:

```cmake
add_subdirectory("/path/to/xcore" xcore)
```

After this, you can link against Xcore target (e.g., xcore) and include headers
from the library.

2. Building for x86 with Unit Tests

Enable unit test building by setting BUILD_TESTING:

```sh
cmake .. -DBUILD_TESTING=ON
make
ctest
```

3. Building for x86 Outside the Project Tree

To install Xcore to a custom location, specify the installation prefix:

```sh
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/output/dir/
make
make install
```

4. Building for Cortex‑M3 Outside the Project Tree

For cross‑compilation to Cortex‑M3, use a predefined toolchain file:

```sh
mkdir build_cortex_m3 && cd build_cortex_m3
cmake .. \
  -DCMAKE_INSTALL_PREFIX=/path/to/output/dir/ \
  -DCMAKE_TOOLCHAIN_FILE=toolchains/cortex-m3.cmake
make
make install
```

## Build Options

Customize the build process using the following CMake options:

* **BUILD_TESTING** — enable unit test compilation and inclusion in the build.
* **XCORE_BITWISE_CRC** — use bitwise CRC implementations to save
  Flash/ROM space (slower but smaller).
* **XCORE_ENTRY_LENGTH** — maximum file name length in file system functions
  (affects internal buffers).
