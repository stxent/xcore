/*
 * xcore/core/cortex/armv6m/asm_base.h
 * Copyright (C) 2023 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_CORTEX_ASM_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_ARMV6M_ASM_BASE_H_
#define XCORE_CORE_CORTEX_ARMV6M_ASM_BASE_H_
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
#endif /* XCORE_CORE_CORTEX_ARMV6M_ASM_BASE_H_ */
