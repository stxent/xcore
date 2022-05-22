/*
 * xcore/core/cortex/memory.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_MEMORY_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_MEMORY_H_
#define XCORE_CORE_CORTEX_MEMORY_H_
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
