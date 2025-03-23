/*
 * xcore/core/riscv/asm.h
 * Copyright (C) 2025 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_ASM_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_RISCV_ASM_H_
#define XCORE_CORE_RISCV_ASM_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
#if __riscv_xlen == 64
typedef uint64_t CSRType;
#else
typedef uint32_t CSRType;
#endif

#define barrier() __asm__ volatile ("" ::: "memory")
/*----------------------------------------------------------------------------*/
/* CSR read instruction */
#define __csrr(number) \
    __extension__ ({ \
      CSRType output; \
      __asm__ volatile ( \
          "CSRR %[output], " EXPAND_TO_STRING(number) \
          : [output] "=r" (output) \
          : \
          : "memory" \
      ); \
      output; \
    })

/* CSR write instruction */
#define __csrw(number, value) \
    do \
    { \
      CSRType input = value; \
      __asm__ volatile ( \
          "csrw " EXPAND_TO_STRING(number) ", %[input]" \
          : \
          : [input] "rK" (input) \
          : "memory" \
      ); \
    } \
    while (0)

/* CSR swap instruction */
#define __csrrw(number, value) \
    __extension__ ({ \
      CSRType result = value; \
      __asm__ volatile ( \
          "csrrw %[input], " EXPAND_TO_STRING(number) ", %[output]" \
          : [input] "=r" (result) \
          : [output] "rK" (result) \
          : "memory" \
      ); \
      result; \
    })

/* CSR clear instruction */
#define __csrc(number, value) \
    do \
    { \
      CSRType input = value; \
      __asm__ volatile ( \
          "csrc " EXPAND_TO_STRING(number) ", %[input]" \
          : \
          : [input] "rK" (input) \
          : "memory" \
      ); \
    } \
    while (0)

/* CSR set instruction */
#define __csrs(number, value) \
    do \
    { \
      CSRType input = value; \
      __asm__ volatile ( \
          "csrs " EXPAND_TO_STRING(number) ", %[input]" \
          : \
          : [input] "rK" (input) \
          : "memory" \
      ); \
    } \
    while (0)

/* CSR read and clear instruction */
#define __csrrc(number, value) \
    __extension__ ({ \
      CSRType result = value; \
      __asm__ volatile ( \
          "csrrc %[input], " EXPAND_TO_STRING(number) ", %[output]" \
          : [input] "=r" (result) \
          : [output] "rK" (result) \
          : "memory" \
      ); \
      result; \
    })

/* CSR read and set instruction */
#define __csrrs(number, value) \
    __extension__ ({ \
      CSRType result = value; \
      __asm__ volatile ( \
          "csrrs %[input], " EXPAND_TO_STRING(number) ", %[output]" \
          : [input] "=r" (result) \
          : [output] "rK" (result) \
          : "memory" \
      ); \
      result; \
    })
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline void __ebreak(void)
{
  __asm__ volatile ("EBREAK");
}

static inline void __ecall(void)
{
  __asm__ volatile ("ECALL" ::: "memory");
}

static inline void __fence_i(void)
{
  __asm__ volatile ("FENCE.I" ::: "memory");
}

static inline void __fence(void)
{
  __asm__ volatile ("FENCE iorw, iorw" ::: "memory");
}

static inline void __nop(void)
{
  __asm__ volatile ("NOP");
}

static inline void __wfi(void)
{
  __asm__ volatile ("WFI" ::: "memory");
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_RISCV_ASM_H_ */
