/*
 * xcore/core/cortex/armv7m/accel.h
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_CORTEX_ACCEL_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_ARMV7M_ACCEL_H_
#define XCORE_CORE_CORTEX_ARMV7M_ACCEL_H_
/*----------------------------------------------------------------------------*/
#include <xcore/asm.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline uint32_t countLeadingZeros32(uint32_t value)
{
  return __clz(value);
}

static inline void invokeDebugger(void)
{
  __bkpt();
}

static inline uint32_t reverseBits32(uint32_t value)
{
  return __rbit(value);
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV7M_ACCEL_H_ */
