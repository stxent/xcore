/*
 * core/cortex/memory.h
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#undef HEADER_PATH
#define HEADER_PATH <core/CORE_TYPE/CORE/memory.h>
#include HEADER_PATH
/*----------------------------------------------------------------------------*/
#define TO_BIG_ENDIAN_16(value) \
    (((value) << 8) & 0xFF00) | (((value) >> 8) & 0x00FF)
#define TO_LITTLE_ENDIAN_16(value)      (value)
#define FROM_BIG_ENDIAN_16(value)       TO_BIG_ENDIAN_16(value)
#define FROM_LITTLE_ENDIAN_16(value)    TO_LITTLE_ENDIAN_16(value)
