/*
 * xcore/core/generic/saturated.h
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_SATURATED_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_GENERIC_SATURATED_H_
#define XCORE_CORE_GENERIC_SATURATED_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <limits.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline long long saturatedAddSLL(long long a, long long b)
{
  if (a > 0)
  {
    if (b > LLONG_MAX - a)
      return LLONG_MAX;
  }
  else
  {
    if (b < LLONG_MIN - a)
      return LLONG_MIN;
  }

  return a + b;
}

static inline long saturatedAddSL(long a, long b)
{
  if (a > 0)
  {
    if (b > LONG_MAX - a)
      return LONG_MAX;
  }
  else
  {
    if (b < LONG_MIN - a)
      return LONG_MIN;
  }

  return a + b;
}

static inline int saturatedAddS(int a, int b)
{
  if (a > 0)
  {
    if (b > INT_MAX - a)
      return INT_MAX;
  }
  else
  {
    if (b < INT_MIN - a)
      return INT_MIN;
  }

  return a + b;
}

static inline short saturatedAddSS(short a, short b)
{
  if (a > 0)
  {
    if (b > SHRT_MAX - a)
      return SHRT_MAX;
  }
  else
  {
    if (b < SHRT_MIN - a)
      return SHRT_MIN;
  }

  return a + b;
}

static inline signed char saturatedAddSC(signed char a, signed char b)
{
  if (a > 0)
  {
    if (b > SCHAR_MAX - a)
      return SCHAR_MAX;
  }
  else
  {
    if (b < SCHAR_MIN - a)
      return SCHAR_MIN;
  }

  return a + b;
}

static inline unsigned long long saturatedAddULL(unsigned long long a,
    unsigned long long b)
{
  return (a > ULLONG_MAX - b) ? ULLONG_MAX : a + b;
}

static inline unsigned long saturatedAddUL(unsigned long a, unsigned long b)
{
  return (a > ULONG_MAX - b) ? ULONG_MAX : a + b;
}

static inline unsigned int saturatedAddU(unsigned int a, unsigned int b)
{
  return (a > UINT_MAX - b) ? UINT_MAX : a + b;
}

static inline unsigned short saturatedAddUS(unsigned short a, unsigned short b)
{
  return (a > USHRT_MAX - b) ? USHRT_MAX : a + b;
}

static inline unsigned char saturatedAddUC(unsigned char a, unsigned char b)
{
  return (a > UCHAR_MAX - b) ? UCHAR_MAX : a + b;
}

static inline long long saturatedSubSLL(long long a, long long b)
{
  if (a < 0)
  {
    if (b > a - LLONG_MIN)
      return LLONG_MIN;
  }
  else
  {
    if (b < a - LLONG_MAX)
      return LLONG_MAX;
  }

  return a - b;
}

static inline long saturatedSubSL(long a, long b)
{
  if (a < 0)
  {
    if (b > a - LONG_MIN)
      return LONG_MIN;
  }
  else
  {
    if (b < a - LONG_MAX)
      return LONG_MAX;
  }

  return a - b;
}

static inline int saturatedSubS(int a, int b)
{
  if (a < 0)
  {
    if (b > a - INT_MIN)
      return INT_MIN;
  }
  else
  {
    if (b < a - INT_MAX)
      return INT_MAX;
  }

  return a - b;
}

static inline short saturatedSubSS(short a, short b)
{
  if (a < 0)
  {
    if (b > a - SHRT_MIN)
      return SHRT_MIN;
  }
  else
  {
    if (b < a - SHRT_MAX)
      return SHRT_MAX;
  }

  return a - b;
}

static inline signed char saturatedSubSC(signed char a, signed char b)
{
  if (a < 0)
  {
    if (b > a - SCHAR_MIN)
      return SCHAR_MIN;
  }
  else
  {
    if (b < a - SCHAR_MAX)
      return SCHAR_MAX;
  }

  return a - b;
}

static inline unsigned long long saturatedSubULL(unsigned long long a,
    unsigned long long b)
{
  return (b > a) ? 0 : a - b;
}

static inline unsigned long saturatedSubUL(unsigned long a, unsigned long b)
{
  return (b > a) ? 0 : a - b;
}

static inline unsigned int saturatedSubU(unsigned int a, unsigned int b)
{
  return (b > a) ? 0 : a - b;
}

static inline unsigned short saturatedSubUS(unsigned short a, unsigned short b)
{
  return (b > a) ? 0 : a - b;
}

static inline unsigned char saturatedSubUC(unsigned char a, unsigned char b)
{
  return (b > a) ? 0 : a - b;
}

END_DECLS
/*----------------------------------------------------------------------------*/
#define saturatedAdd(valueA, valueB) _Generic((valueA), \
    signed long long: saturatedAddSLL, \
    signed long: saturatedAddSL, \
    signed int: saturatedAddS, \
    signed short: saturatedAddSS, \
    signed char: saturatedAddSC, \
    unsigned long long: saturatedAddULL, \
    unsigned long: saturatedAddUL, \
    unsigned int: saturatedAddU, \
    unsigned short: saturatedAddUS, \
    unsigned char: saturatedAddUC \
)((valueA), (valueB))

#define saturatedSub(valueA, valueB) _Generic((valueA), \
    signed long long: saturatedSubSLL, \
    signed long: saturatedSubSL, \
    signed int: saturatedSubS, \
    signed short: saturatedSubSS, \
    signed char: saturatedSubSC, \
    unsigned long long: saturatedSubULL, \
    unsigned long: saturatedSubUL, \
    unsigned int: saturatedSubU, \
    unsigned short: saturatedSubUS, \
    unsigned char: saturatedSubUC \
)((valueA), (valueB))
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_GENERIC_ATOMIC_H_ */
