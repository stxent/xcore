/*
 * xcore/core/x86/accel.h
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_ACCEL_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_X86_ACCEL_H_
#define XCORE_CORE_X86_ACCEL_H_
/*----------------------------------------------------------------------------*/
#include "../generic/accel_rev.h"
#include "../generic/accel_sat.h"
#include <stdint.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline uint32_t countLeadingZeros32(uint32_t value)
{
  /* If the input value is 0, the result is undefined */
  return __builtin_clz(value);
}

static inline void invokeDebugger(void)
{
}

static inline void invokeSupervisor(void)
{
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_X86_ACCEL_H_ */
