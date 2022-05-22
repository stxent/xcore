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

static inline unsigned long long atomicFetchAddULL(
    volatile unsigned long long *pointer, unsigned long long value)
{
  return __atomic_fetch_add(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long atomicFetchAddUL(volatile unsigned long *pointer,
    unsigned long value)
{
  return __atomic_fetch_add(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned int atomicFetchAddU(volatile unsigned int *pointer,
    unsigned int value)
{
  return __atomic_fetch_add(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned short atomicFetchAddUS(volatile unsigned short *pointer,
    unsigned short value)
{
  return __atomic_fetch_add(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned char atomicFetchAddUC(volatile unsigned char *pointer,
    unsigned char value)
{
  return __atomic_fetch_add(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long long atomicFetchAndULL(
    volatile unsigned long long *pointer, unsigned long long value)
{
  return __atomic_fetch_and(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long atomicFetchAndUL(volatile unsigned long *pointer,
    unsigned long value)
{
  return __atomic_fetch_and(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned int atomicFetchAndU(volatile unsigned int *pointer,
    unsigned int value)
{
  return __atomic_fetch_and(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned short atomicFetchAndUS(volatile unsigned short *pointer,
    unsigned short value)
{
  return __atomic_fetch_and(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned char atomicFetchAndUC(volatile unsigned char *pointer,
    unsigned char value)
{
  return __atomic_fetch_and(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long long atomicFetchOrULL(
    volatile unsigned long long *pointer, unsigned long long value)
{
  return __atomic_fetch_or(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long atomicFetchOrUL(volatile unsigned long *pointer,
    unsigned long value)
{
  return __atomic_fetch_or(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned int atomicFetchOrU(volatile unsigned int *pointer,
    unsigned int value)
{
  return __atomic_fetch_or(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned short atomicFetchOrUS(volatile unsigned short *pointer,
    unsigned short value)
{
  return __atomic_fetch_or(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned char atomicFetchOrUC(volatile unsigned char *pointer,
    unsigned char value)
{
  return __atomic_fetch_or(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long long atomicFetchSubULL(
    volatile unsigned long long *pointer, unsigned long long value)
{
  return __atomic_fetch_sub(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned long atomicFetchSubUL(volatile unsigned long *pointer,
    unsigned long value)
{
  return __atomic_fetch_sub(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned int atomicFetchSubU(volatile unsigned int *pointer,
    unsigned int value)
{
  return __atomic_fetch_sub(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned short atomicFetchSubUS(volatile unsigned short *pointer,
    unsigned short value)
{
  return __atomic_fetch_sub(pointer, value, __ATOMIC_SEQ_CST);
}

static inline unsigned char atomicFetchSubUC(volatile unsigned char *pointer,
    unsigned char value)
{
  return __atomic_fetch_sub(pointer, value, __ATOMIC_SEQ_CST);
}

static inline bool compareExchangePointer(volatile void *pointer,
    void *expected, void *desired)
{
  return __atomic_compare_exchange((void * volatile *)pointer,
      (void **)expected, &desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

END_DECLS
/*----------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_GENERIC_ATOMIC_H_ */
