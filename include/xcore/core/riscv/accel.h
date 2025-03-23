/*
 * xcore/core/riscv/accel.h
 * Copyright (C) 2025 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_ACCEL_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_RISCV_ACCEL_H_
#define XCORE_CORE_RISCV_ACCEL_H_
/*----------------------------------------------------------------------------*/
#include "../generic/accel_sat.h"
#include <xcore/asm.h>
/*----------------------------------------------------------------------------*/
static inline uint32_t countLeadingZeros32(uint32_t value)
{
  /* If the input value is 0, the result is undefined */
  return __builtin_clz(value);
}

static inline void invokeDebugger(void)
{
  __ebreak();
}

static inline void invokeSupervisor(void)
{
  __ecall();
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
  value = __builtin_bswap32(value);

  return value;
}
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_RISCV_ACCEL_H_ */
