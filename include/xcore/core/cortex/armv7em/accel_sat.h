/*
 * xcore/core/cortex/armv7em/accel_sat.h
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_CORTEX_ACCEL_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_ARMV7EM_ACCEL_SAT_H_
#define XCORE_CORE_CORTEX_ARMV7EM_ACCEL_SAT_H_
/*----------------------------------------------------------------------------*/
#include <xcore/asm.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline long saturatedAddSL(long a, long b)
{
  return (long)__qadd((uint32_t)a, (uint32_t)b);
}

static inline int saturatedAddS(int a, int b)
{
  return (int)__qadd((uint32_t)a, (uint32_t)b);
}

static inline short saturatedAddSS(short a, short b)
{
  /* QADD16 is parallel 2x16-bit instruction, only lower half-word is used */
  return (short)__qadd16((uint32_t)a, (uint32_t)b);
}

static inline signed char saturatedAddSC(signed char a, signed char b)
{
  /* QADD8 is parallel 4x8-bit instruction, only lower byte is used */
  return (signed char)__qadd8((uint32_t)a, (uint32_t)b);
}

static inline unsigned short saturatedAddUS(unsigned short a, unsigned short b)
{
  /* UQADD16 is parallel 2x16-bit instruction, only lower half-word is used */
  return (unsigned short)__uqadd16((uint32_t)a, (uint32_t)b);
}

static inline unsigned char saturatedAddUC(unsigned char a, unsigned char b)
{
  /* UQADD8 is parallel 4x8-bit instruction, only lower byte is used */
  return (unsigned char)__uqadd8((uint32_t)a, (uint32_t)b);
}

static inline long saturatedSubSL(long a, long b)
{
  return (long)__qsub((uint32_t)a, (uint32_t)b);
}

static inline int saturatedSubS(int a, int b)
{
  return (int)__qsub((uint32_t)a, (uint32_t)b);
}

static inline short saturatedSubSS(short a, short b)
{
  /* QSUB16 is parallel 2x16-bit instruction, only lower half-word is used */
  return (short)__qsub16((uint32_t)a, (uint32_t)b);
}

static inline signed char saturatedSubSC(signed char a, signed char b)
{
  /* QSUB8 is parallel 4x8-bit instruction, only lower byte is used */
  return (signed char)__qsub8((uint32_t)a, (uint32_t)b);
}

static inline unsigned short saturatedSubUS(unsigned short a, unsigned short b)
{
  /* UQSUB16 is parallel 2x16-bit instruction, only lower half-word is used */
  return (unsigned short)__uqsub16((uint32_t)a, (uint32_t)b);
}

static inline unsigned char saturatedSubUC(unsigned char a, unsigned char b)
{
  /* UQSUB8 is parallel 4x8-bit instruction, only lower byte is used */
  return (unsigned char)__uqsub8((uint32_t)a, (uint32_t)b);
}

END_DECLS
/*----------------------------------------------------------------------------*/
#define saturatedAdd(valueA, valueB) _Generic((valueA), \
    signed long: saturatedAddSL, \
    signed int: saturatedAddS, \
    signed short: saturatedAddSS, \
    signed char: saturatedAddSC, \
    unsigned short: saturatedAddUS, \
    unsigned char: saturatedAddUC \
)((valueA), (valueB))

#define saturatedSub(valueA, valueB) _Generic((valueA), \
    signed long: saturatedSubSL, \
    signed int: saturatedSubS, \
    signed short: saturatedSubSS, \
    signed char: saturatedSubSC, \
    unsigned short: saturatedSubUS, \
    unsigned char: saturatedSubUC \
)((valueA), (valueB))
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV7EM_ACCEL_SAT_H_ */
