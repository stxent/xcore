/*
 * xcore/memory.h
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_MEMORY_H_
#define XCORE_MEMORY_H_
/*----------------------------------------------------------------------------*/
#include <xcore/target.h>
/*----------------------------------------------------------------------------*/
#define TO_BIG_ENDIAN_16(value) \
    ((((value) & 0x00FFU) << 8) | (((value) & 0xFF00U) >> 8))
#define TO_LITTLE_ENDIAN_16(value)      (value)
#define FROM_BIG_ENDIAN_16(value)       TO_BIG_ENDIAN_16(value)
#define FROM_LITTLE_ENDIAN_16(value)    TO_LITTLE_ENDIAN_16(value)

#define TO_BIG_ENDIAN_32(value) \
    ((((value) & 0x000000FFUL) << 24) | (((value) & 0x0000FF00UL) << 8) \
        | (((value) & 0x00FF0000UL) >> 8) | (((value) & 0xFF000000UL) >> 24))
#define TO_LITTLE_ENDIAN_32(value)      (value)
#define FROM_BIG_ENDIAN_32(value)       TO_BIG_ENDIAN_32(value)
#define FROM_LITTLE_ENDIAN_32(value)    TO_LITTLE_ENDIAN_32(value)
/*----------------------------------------------------------------------------*/
#undef HEADER_PATH
#define HEADER_PATH <xcore/core/CORE_TYPE/memory.h>
#include HEADER_PATH
#undef HEADER_PATH
/*----------------------------------------------------------------------------*/
#endif /* XCORE_MEMORY_H_ */
