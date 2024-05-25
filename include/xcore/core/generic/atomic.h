/*
 * xcore/core/generic/atomic.h
 * Copyright (C) 2022 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_ATOMIC_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_GENERIC_ATOMIC_H_
#define XCORE_CORE_GENERIC_ATOMIC_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stdbool.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline bool atomicCompareExchangeULL(unsigned long long *pointer,
    unsigned long long *expected, unsigned long long desired)
{
  return __atomic_compare_exchange_n(pointer, expected,
      desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

static inline bool atomicCompareExchangeUL(unsigned long *pointer,
    unsigned long *expected, unsigned long desired)
{
  return __atomic_compare_exchange_n(pointer, expected,
      desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

static inline bool atomicCompareExchangeU(unsigned int *pointer,
    unsigned int *expected, unsigned int desired)
{
  return __atomic_compare_exchange_n(pointer, expected,
      desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

static inline bool atomicCompareExchangeUS(unsigned short *pointer,
    unsigned short *expected, unsigned short desired)
{
  return __atomic_compare_exchange_n(pointer, expected,
      desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

static inline bool atomicCompareExchangeUC(unsigned char *pointer,
    unsigned char *expected, unsigned char desired)
{
  return __atomic_compare_exchange_n(pointer, expected,
      desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

static inline unsigned long long atomicFetchAddULL(unsigned long long *pointer,
    unsigned long long value)
{
  return __atomic_fetch_add(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long atomicFetchAddUL(unsigned long *pointer,
    unsigned long value)
{
  return __atomic_fetch_add(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned int atomicFetchAddU(unsigned int *pointer,
    unsigned int value)
{
  return __atomic_fetch_add(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned short atomicFetchAddUS(unsigned short *pointer,
    unsigned short value)
{
  return __atomic_fetch_add(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned char atomicFetchAddUC(unsigned char *pointer,
    unsigned char value)
{
  return __atomic_fetch_add(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long long atomicFetchAndULL(unsigned long long *pointer,
    unsigned long long value)
{
  return __atomic_fetch_and(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long atomicFetchAndUL(unsigned long *pointer,
    unsigned long value)
{
  return __atomic_fetch_and(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned int atomicFetchAndU(unsigned int *pointer,
    unsigned int value)
{
  return __atomic_fetch_and(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned short atomicFetchAndUS(unsigned short *pointer,
    unsigned short value)
{
  return __atomic_fetch_and(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned char atomicFetchAndUC(unsigned char *pointer,
    unsigned char value)
{
  return __atomic_fetch_and(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long long atomicFetchOrULL(unsigned long long *pointer,
    unsigned long long value)
{
  return __atomic_fetch_or(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long atomicFetchOrUL(unsigned long *pointer,
    unsigned long value)
{
  return __atomic_fetch_or(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned int atomicFetchOrU(unsigned int *pointer,
    unsigned int value)
{
  return __atomic_fetch_or(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned short atomicFetchOrUS(unsigned short *pointer,
    unsigned short value)
{
  return __atomic_fetch_or(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned char atomicFetchOrUC(unsigned char *pointer,
    unsigned char value)
{
  return __atomic_fetch_or(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long long atomicFetchSubULL(unsigned long long *pointer,
    unsigned long long value)
{
  return __atomic_fetch_sub(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long atomicFetchSubUL(unsigned long *pointer,
    unsigned long value)
{
  return __atomic_fetch_sub(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned int atomicFetchSubU(unsigned int *pointer,
    unsigned int value)
{
  return __atomic_fetch_sub(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned short atomicFetchSubUS(unsigned short *pointer,
    unsigned short value)
{
  return __atomic_fetch_sub(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned char atomicFetchSubUC(unsigned char *pointer,
    unsigned char value)
{
  return __atomic_fetch_sub(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long long atomicLoadULL(
    const unsigned long long *pointer)
{
  return __atomic_load_n(pointer, __ATOMIC_SEQ_CST);
}

static inline unsigned long atomicLoadUL(const unsigned long *pointer)
{
  return __atomic_load_n(pointer, __ATOMIC_SEQ_CST);
}

static inline unsigned int atomicLoadU(const unsigned int *pointer)
{
  return __atomic_load_n(pointer, __ATOMIC_SEQ_CST);
}

static inline unsigned short atomicLoadUS(const unsigned short *pointer)
{
  return __atomic_load_n(pointer, __ATOMIC_SEQ_CST);
}

static inline unsigned char atomicLoadUC(const unsigned char *pointer)
{
  return __atomic_load_n(pointer, __ATOMIC_SEQ_CST);
}

static inline bool compareExchangePointer(void *pointer, void *expected,
    void *desired)
{
  return __atomic_compare_exchange_n((void **)pointer, (void **)expected,
      desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

END_DECLS
/*----------------------------------------------------------------------------*/
#define atomicCompareExchange(pointer, expected, desired) _Generic((pointer), \
    unsigned long long *: atomicCompareExchangeULL, \
    unsigned long *: atomicCompareExchangeUL, \
    unsigned int *: atomicCompareExchangeU, \
    unsigned short *: atomicCompareExchangeUS, \
    unsigned char *: atomicCompareExchangeUC \
)((pointer), (expected), (desired))

#define atomicFetchAdd(pointer, value) _Generic((pointer), \
    unsigned long long *: atomicFetchAddULL, \
    unsigned long *: atomicFetchAddUL, \
    unsigned int *: atomicFetchAddU, \
    unsigned short *: atomicFetchAddUS, \
    unsigned char *: atomicFetchAddUC \
)((pointer), (value))

#define atomicFetchAnd(pointer, value) _Generic((pointer), \
    unsigned long long *: atomicFetchAndULL, \
    unsigned long *: atomicFetchAndUL, \
    unsigned int *: atomicFetchAndU, \
    unsigned short *: atomicFetchAndUS, \
    unsigned char *: atomicFetchAndUC \
)((pointer), (value))

#define atomicFetchOr(pointer, value) _Generic((pointer), \
    unsigned long long *: atomicFetchOrULL, \
    unsigned long *: atomicFetchOrUL, \
    unsigned int *: atomicFetchOrU, \
    unsigned short *: atomicFetchOrUS, \
    unsigned char *: atomicFetchOrUC \
)((pointer), (value))

#define atomicFetchSub(pointer, value) _Generic((pointer), \
    unsigned long long *: atomicFetchSubULL, \
    unsigned long *: atomicFetchSubUL, \
    unsigned int *: atomicFetchSubU, \
    unsigned short *: atomicFetchSubUS, \
    unsigned char *: atomicFetchSubUC \
)((pointer), (value))

#define atomicLoad(pointer) _Generic((pointer), \
    const unsigned long long *: atomicLoadULL, \
    unsigned long long *: atomicLoadULL, \
    const unsigned long *: atomicLoadUL, \
    unsigned long *: atomicLoadUL, \
    const unsigned int *: atomicLoadU, \
    unsigned int *: atomicLoadU, \
    const unsigned short *: atomicLoadUS, \
    unsigned short *: atomicLoadUS, \
    const unsigned char *: atomicLoadUC, \
    unsigned char *: atomicLoadUC \
)(pointer)
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_GENERIC_ATOMIC_H_ */
