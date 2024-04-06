/*
 * xcore/core/cortex/saturated.h
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_SATURATED_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_SATURATED_H_
#define XCORE_CORE_CORTEX_SATURATED_H_
/*----------------------------------------------------------------------------*/
#if defined(__ARM_ARCH_6M__)
#  include "armv6m/saturated.h"
#elif defined(__ARM_ARCH_7M__)
#  include "armv7m/saturated.h"
#elif defined(__ARM_ARCH_7EM__)
#  include "armv7em/saturated.h"
#endif
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_SATURATED_H_ */
