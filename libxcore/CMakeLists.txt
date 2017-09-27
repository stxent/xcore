#Copyright (C) 2017 xent
#Project is distributed under the terms of the GNU General Public License v3.0

file(GLOB LIBRARY_SOURCES
        "${PROJECT_SOURCE_DIR}/libxcore/*.c"
        "${PROJECT_SOURCE_DIR}/libxcore/containers/*.c"
        "${PROJECT_SOURCE_DIR}/libxcore/crc/*.c"
)

include("${PROJECT_SOURCE_DIR}/libxcore/core/CMakeLists.txt")
