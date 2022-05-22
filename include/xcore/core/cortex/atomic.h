/*
 * xcore/core/cortex/atomic.h
 * Copyright (C) 2022 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_ATOMIC_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_ATOMIC_H_
#define XCORE_CORE_CORTEX_ATOMIC_H_
/*----------------------------------------------------------------------------*/
#if defined(__ARM_ARCH_6M__)
#  include "armv6m/atomic.h"
#elif defined(__ARM_ARCH_7M__)
#  include "armv7m/atomic.h"
#elif defined(__ARM_ARCH_7EM__)
#  include "armv7em/atomic.h"
#endif
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ATOMIC_H_ */
