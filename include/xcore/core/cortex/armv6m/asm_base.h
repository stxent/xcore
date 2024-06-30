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

static inline void __bkpt(void)
{
  __asm__ volatile ("BKPT #0");
}

static inline void __cpsid(void)
{
  __asm__ volatile ("CPSID i");
}

static inline void __cpsid_fault(void)
{
  __asm__ volatile ("CPSID f");
}

static inline void __cpsie(void)
{
  __asm__ volatile ("CPSIE i");
}

static inline void __cpsie_fault(void)
{
  __asm__ volatile ("CPSIE f");
}

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

static inline uint32_t __mrs_primask(void)
{
  uint32_t state;

  __asm__ volatile (
      "MRS %[state], PRIMASK"
      : [state] "=r" (state)
  );
  return state;
}

static inline void __msr_primask(uint32_t state)
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

static inline void __svc(void)
{
  __asm__ volatile ("SVC #0");
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

[[gnu::naked]] uint32_t __getMainStackPointer(void);
[[gnu::naked]] uint32_t __getProcessStackPointer(void);
[[gnu::naked]] void __setMainStackPointer(uint32_t);
[[gnu::naked]] void __setProcessStackPointer(uint32_t);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV6M_ASM_BASE_H_ */
