/*
 * xcore/core/cortex/armv6m/accel.h
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_CORTEX_ACCEL_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_ARMV6M_ACCEL_H_
#define XCORE_CORE_CORTEX_ARMV6M_ACCEL_H_
/*----------------------------------------------------------------------------*/
#include "../../generic/accel_clz.h"
#include "../../generic/accel_sat.h"
#include <xcore/asm.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline void invokeDebugger(void)
{
  __bkpt();
}

static inline uint32_t reverseBits32(uint32_t value)
{
  /* Swap odd and even bits */
  value = ((value >> 1) & 0x55555555UL) | ((value & 0x55555555UL) << 1);
  /* Swap consecutive pairs */
  value = ((value >> 2) & 0x33333333UL) | ((value & 0x33333333UL) << 2);
  /* Swap nibbles */
  value = ((value >> 4) & 0x0F0F0F0FUL) | ((value & 0x0F0F0F0FUL) << 4);
  /* Swap bytes and pairs */
  value = __rev(value);

  return value;
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV6M_ACCEL_H_ */
