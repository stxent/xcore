/*
 * xcore/core/cortex/armv7m/asm.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_CORTEX_ASM_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_ARMV7M_ASM_H_
#define XCORE_CORE_CORTEX_ARMV7M_ASM_H_
/*----------------------------------------------------------------------------*/
#include "../armv6m/asm_base.h"
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline void __clrex(void)
{
  __asm__ volatile ("CLREX");
}

static inline uint32_t __ldrex(volatile uint32_t *address)
{
  uint32_t result;

  __asm__ volatile (
      "LDREX %[result], [%[address]]"
      : [result] "=r" (result)
      : [address] "r" (address)
  );
  return result;
}

static inline uint8_t __ldrexb(volatile uint8_t *address)
{
  uint8_t result;

  __asm__ volatile (
      "LDREXB %[result], [%[address]]"
      : [result] "=r" (result)
      : [address] "r" (address)
  );
  return result;
}

static inline uint16_t __ldrexh(volatile uint16_t *address)
{
  uint16_t result;

  __asm__ volatile (
      "LDREXH %[result], [%[address]]"
      : [result] "=r" (result)
      : [address] "r" (address)
  );
  return result;
}

static inline uint32_t __strex(uint32_t value, volatile uint32_t *address)
{
  uint32_t result;

  __asm__ volatile (
      "STREX %[result], %[value], [%[address]]"
      : [result] "=&r" (result)
      : [address] "r" (address), [value] "r" (value)
  );
  return result;
}

static inline uint32_t __strexb(uint8_t value, volatile uint8_t *address)
{
  uint32_t result;

  __asm__ volatile (
      "STREXB %[result], %[value], [%[address]]"
      : [result] "=&r" (result)
      : [address] "r" (address), [value] "r" (value)
  );
  return result;
}

static inline uint32_t __strexh(uint16_t value, volatile uint16_t *address)
{
  uint32_t result;

  __asm__ volatile (
      "STREXH %[result], %[value], [%[address]]"
      : [result] "=&r" (result)
      : [address] "r" (address), [value] "r" (value)
  );
  return result;
}

static inline uint32_t __mrs_basepri(void)
{
  uint32_t priority;

  __asm__ volatile (
      "MRS %[priority], BASEPRI"
      : [priority] "=r" (priority)
  );
  return priority;
}

// static inline uint32_t __interruptsGetBasePriority(void)
// {
//   uint32_t priority;

//   __asm__ volatile (
//       "MRS %[priority], BASEPRI"
//       : [priority] "=r" (priority)
//   );
//   return priority;
// }

static inline void __msr_basepri_max(uint32_t priority)
{
  __asm__ volatile (
      "MSR BASEPRI_MAX, %[priority]"
      :
      : [priority] "r" (priority)
  );
}

// static inline void __interruptsSetBasePriority(uint32_t priority)
// {
//   __asm__ volatile (
//       "MSR BASEPRI_MAX, %[priority]"
//       :
//       : [priority] "r" (priority)
//   );
// }

static inline void __msr_basepri(uint32_t priority)
{
  __asm__ volatile (
      "MSR BASEPRI, %[priority]"
      :
      : [priority] "r" (priority)
  );
}

// static inline void __interruptsResetBasePriority(uint32_t priority)
// {
//   __asm__ volatile (
//       "MSR BASEPRI, %[priority]"
//       :
//       : [priority] "r" (priority)
//   );
// }

static inline uint32_t __clz(uint32_t value)
{
  uint32_t result;

  __asm__ volatile (
      "CLZ %[result], %[value]"
      : [result] "=r" (result)
      : [value] "r" (value)
  );
  return result;
}

static inline uint32_t __rbit(uint32_t value)
{
  uint32_t result;

  __asm__ volatile (
      "RBIT %[result], %[value]"
      : [result] "=r" (result)
      : [value] "r" (value)
  );
  return result;
}

static inline uint32_t __rev(uint32_t value)
{
  uint32_t result;

  __asm__ volatile (
      "REV %[result], %[value]"
      : [result] "=r" (result)
      : [value] "r" (value)
  );
  return result;
}

static inline uint32_t __rev16(uint32_t value)
{
  uint32_t result;

  __asm__ volatile (
      "REV16 %[result], %[value]"
      : [result] "=r" (result)
      : [value] "r" (value)
  );
  return result;
}

static inline uint32_t __revsh(uint32_t value)
{
  uint32_t result;

  __asm__ volatile (
      "REVSH %[result], %[value]"
      : [result] "=r" (result)
      : [value] "r" (value)
  );
  return result;
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV7M_ASM_H_ */
