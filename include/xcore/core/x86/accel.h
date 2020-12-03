/*
 * xcore/core/x86/accel.h
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_X86_ACCEL_H_
#define XCORE_CORE_X86_ACCEL_H_
/*----------------------------------------------------------------------------*/
#include <stdint.h>
/*----------------------------------------------------------------------------*/
static inline uint32_t countLeadingZeros32(uint32_t value)
{
  /* If the input value is 0, the result is undefined */
  return __builtin_clz(value);
}
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_X86_ACCEL_H_ */
