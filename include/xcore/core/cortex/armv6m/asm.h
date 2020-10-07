/*
 * xcore/core/cortex/armv6m/asm.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_CORTEX_ARMV6M_ASM_H_
#define XCORE_CORE_CORTEX_ARMV6M_ASM_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline void __dmb(void)
{
  __asm__ volatile ("DMB");
}

static inline void __dsb(void)
{
  __asm__ volatile ("DSB");
}

static inline void __isb(void)
{
  __asm__ volatile ("ISB");
}

static inline void __interruptsDisable(void)
{
  __asm__ volatile ("CPSID i");
}

static inline void __interruptsEnable(void)
{
  __asm__ volatile ("CPSIE i");
}

static inline uint32_t __interruptsGetState(void)
{
  uint32_t state;

  __asm__ volatile (
      "MRS %[state], PRIMASK"
      : [state] "=r" (state)
  );
  return state;
}

static inline void __interruptsSetState(uint32_t state)
{
  __asm__ volatile (
      "MSR PRIMASK, %[state]"
      :
      : [state] "r" (state)
  );
}

static inline void __nop(void)
{
  __asm__ volatile ("NOP");
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

static inline void __wfe(void)
{
  __asm__ volatile ("WFE");
}

static inline void __wfi(void)
{
  __asm__ volatile ("WFI");
}

END_DECLS
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

uint32_t __getMainStackPointer(void) __attribute__((naked));
uint32_t __getProcessStackPointer(void) __attribute__((naked));
void __setMainStackPointer(uint32_t) __attribute__((naked));
void __setProcessStackPointer(uint32_t) __attribute__((naked));

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV6M_ASM_H_ */
