/*
 * xcore/core/cortex/armv7em/asm.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_CORTEX_ASM_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_ARMV7EM_ASM_H_
#define XCORE_CORE_CORTEX_ARMV7EM_ASM_H_
/*----------------------------------------------------------------------------*/
#include "../armv7m/asm.h"
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline uint32_t __qadd(uint32_t valueA, uint32_t valueB)
{
  uint32_t result;

  __asm__ volatile (
      "QADD %[result], %[valueA], %[valueB]"
      : [result] "=r" (result)
      : [valueA] "r" (valueA), [valueB] "r" (valueB)
  );
  return result;
}

static inline uint32_t __qadd8(uint32_t valueA, uint32_t valueB)
{
  uint32_t result;

  __asm__ volatile (
      "QADD8 %[result], %[valueA], %[valueB]"
      : [result] "=r" (result)
      : [valueA] "r" (valueA), [valueB] "r" (valueB)
  );
  return result;
}

static inline uint32_t __qadd16(uint32_t valueA, uint32_t valueB)
{
  uint32_t result;

  __asm__ volatile (
      "QADD16 %[result], %[valueA], %[valueB]"
      : [result] "=r" (result)
      : [valueA] "r" (valueA), [valueB] "r" (valueB)
  );
  return result;
}

static inline uint32_t __qsub(uint32_t valueA, uint32_t valueB)
{
  uint32_t result;

  __asm__ volatile (
      "QSUB %[result], %[valueA], %[valueB]"
      : [result] "=r" (result)
      : [valueA] "r" (valueA), [valueB] "r" (valueB)
  );
  return result;
}

static inline uint32_t __qsub8(uint32_t valueA, uint32_t valueB)
{
  uint32_t result;

  __asm__ volatile (
      "QSUB8 %[result], %[valueA], %[valueB]"
      : [result] "=r" (result)
      : [valueA] "r" (valueA), [valueB] "r" (valueB)
  );
  return result;
}

static inline uint32_t __qsub16(uint32_t valueA, uint32_t valueB)
{
  uint32_t result;

  __asm__ volatile (
      "QSUB16 %[result], %[valueA], %[valueB]"
      : [result] "=r" (result)
      : [valueA] "r" (valueA), [valueB] "r" (valueB)
  );
  return result;
}

static inline uint32_t __uqadd8(uint32_t valueA, uint32_t valueB)
{
  uint32_t result;

  __asm__ volatile (
      "UQADD8 %[result], %[valueA], %[valueB]"
      : [result] "=r" (result)
      : [valueA] "r" (valueA), [valueB] "r" (valueB)
  );
  return result;
}

static inline uint32_t __uqadd16(uint32_t valueA, uint32_t valueB)
{
  uint32_t result;

  __asm__ volatile (
      "UQADD16 %[result], %[valueA], %[valueB]"
      : [result] "=r" (result)
      : [valueA] "r" (valueA), [valueB] "r" (valueB)
  );
  return result;
}

static inline uint32_t __uqsub8(uint32_t valueA, uint32_t valueB)
{
  uint32_t result;

  __asm__ volatile (
      "UQSUB8 %[result], %[valueA], %[valueB]"
      : [result] "=r" (result)
      : [valueA] "r" (valueA), [valueB] "r" (valueB)
  );
  return result;
}

static inline uint32_t __uqsub16(uint32_t valueA, uint32_t valueB)
{
  uint32_t result;

  __asm__ volatile (
      "UQSUB16 %[result], %[valueA], %[valueB]"
      : [result] "=r" (result)
      : [valueA] "r" (valueA), [valueB] "r" (valueB)
  );
  return result;
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV7EM_ASM_H_ */
