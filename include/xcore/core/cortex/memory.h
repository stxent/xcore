/*
 * xcore/core/cortex/memory.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_CORTEX_MEMORY_H_
#define XCORE_CORE_CORTEX_MEMORY_H_
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
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
#if defined(__ARM_ARCH_6M__)
#  include "armv6m/memory.h"
#elif defined(__ARM_ARCH_7M__)
#  include "armv7m/memory.h"
#elif defined(__ARM_ARCH_7EM__)
#  include "armv7em/memory.h"
#endif
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_MEMORY_H_ */
