/*
 * xcore/core/x86/asm.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_INTEL_ASM_H_
#define XCORE_CORE_INTEL_ASM_H_
/*----------------------------------------------------------------------------*/
#include <stdint.h>
/*----------------------------------------------------------------------------*/
#define barrier() __asm__ volatile ("" : : : "memory")
/*----------------------------------------------------------------------------*/
static inline uint32_t countLeadingZeros32(uint32_t value)
{
  /* If the input value is 0, the result is undefined */
  return __builtin_clz(value);
}
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_INTEL_ASM_H_ */
