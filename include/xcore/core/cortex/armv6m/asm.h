/*
 * xcore/core/cortex/armv6m/asm.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_CORTEX_ASM_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_ARMV6M_ASM_H_
#define XCORE_CORE_CORTEX_ARMV6M_ASM_H_
/*----------------------------------------------------------------------------*/
#include "asm_base.h"
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline uint32_t __rev(uint32_t value)
{
  uint32_t result;

  __asm__ volatile (
      "REV %[result], %[value]"
      : [result] "=l" (result)
      : [value] "l" (value)
  );
  return result;
}

static inline uint32_t __rev16(uint32_t value)
{
  uint32_t result;

  __asm__ volatile (
      "REV16 %[result], %[value]"
      : [result] "=l" (result)
      : [value] "l" (value)
  );
  return result;
}

static inline uint32_t __revsh(uint32_t value)
{
  uint32_t result;

  __asm__ volatile (
      "REVSH %[result], %[value]"
      : [result] "=l" (result)
      : [value] "l" (value)
  );
  return result;
}

END_DECLS
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

[[gnu::naked]] uint32_t __getMainStackPointer(void);
[[gnu::naked]] uint32_t __getProcessStackPointer(void);
[[gnu::naked]] void __setMainStackPointer(uint32_t);
[[gnu::naked]] void __setProcessStackPointer(uint32_t);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV6M_ASM_H_ */
