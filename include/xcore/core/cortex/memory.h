/*
 * xcore/core/cortex/memory.h
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_CORTEX_MEMORY_H_
#define XCORE_CORE_CORTEX_MEMORY_H_
/*----------------------------------------------------------------------------*/
#undef HEADER_PATH
#define HEADER_PATH <xcore/core/CORE_TYPE/CORE/memory.h>
#include HEADER_PATH
/*----------------------------------------------------------------------------*/
#define TO_BIG_ENDIAN_16(value) \
    ((((value) << 8) & 0xFF00) | (((value) >> 8) & 0x00FF))
#define TO_LITTLE_ENDIAN_16(value)      (value)
#define FROM_BIG_ENDIAN_16(value)       TO_BIG_ENDIAN_16(value)
#define FROM_LITTLE_ENDIAN_16(value)    TO_LITTLE_ENDIAN_16(value)

#define TO_BIG_ENDIAN_32(value) \
    ((((value) << 24) & 0xFF000000) | (((value) << 8) & 0x00FF0000) \
    | (((value) >> 24) & 0x000000FF) | (((value) >> 8) & 0x0000FF00))
#define TO_LITTLE_ENDIAN_32(value)      (value)
#define FROM_BIG_ENDIAN_32(value)       TO_BIG_ENDIAN_32(value)
#define FROM_LITTLE_ENDIAN_32(value)    TO_LITTLE_ENDIAN_32(value)
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_MEMORY_H_ */
