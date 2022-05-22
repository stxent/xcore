/*
 * xcore/core/cortex/armv6m/atomic.h
 * Copyright (C) 2022 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_CORTEX_ATOMIC_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_ARMV6M_ATOMIC_H_
#define XCORE_CORE_CORTEX_ARMV6M_ATOMIC_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stdbool.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

unsigned long atomicFetchAddUL(volatile unsigned long *, unsigned long);
unsigned int atomicFetchAddU(volatile unsigned int *, unsigned int);
unsigned short atomicFetchAddUS(volatile unsigned short *, unsigned short);
unsigned char atomicFetchAddUC(volatile unsigned char *, unsigned char);

unsigned long atomicFetchAndUL(volatile unsigned long *, unsigned long);
unsigned int atomicFetchAndU(volatile unsigned int *, unsigned int);
unsigned short atomicFetchAndUS(volatile unsigned short *, unsigned short);
unsigned char atomicFetchAndUC(volatile unsigned char *, unsigned char);

unsigned long atomicFetchOrUL(volatile unsigned long *, unsigned long);
unsigned int atomicFetchOrU(volatile unsigned int *, unsigned int);
unsigned short atomicFetchOrUS(volatile unsigned short *, unsigned short);
unsigned char atomicFetchOrUC(volatile unsigned char *, unsigned char);

unsigned long atomicFetchSubUL(volatile unsigned long *, unsigned long);
unsigned int atomicFetchSubU(volatile unsigned int *, unsigned int);
unsigned short atomicFetchSubUS(volatile unsigned short *, unsigned short);
unsigned char atomicFetchSubUC(volatile unsigned char *, unsigned char);

/**
 * This function implements an atomic compare and swap operation.
 * This compares the contents of @b pointer with the contents of @b expected.
 * If equal, the operation is a read-modify-write operation that writes
 * a desired value into @b pointer and @b true is returned. Otherwise,
 * the current contents of @b pointer are written into @b expected
 * and @b false is returned.
 * @param pointer Pointer to the contents to be modified.
 * @param expected Pointer to the expected value.
 * @param desired Desired value.
 * @return @b true if a desired value is written into @b pointer
 * and @false otherwise.
 */
bool compareExchangePointer(volatile void *, void *, void *);

END_DECLS
/*----------------------------------------------------------------------------*/
#define atomicFetchAdd(pointer, value) _Generic((pointer), \
    unsigned long *: atomicFetchAddUL, \
    unsigned int *: atomicFetchAddU, \
    unsigned short *: atomicFetchAddUS, \
    unsigned char *: atomicFetchAddUC \
)((pointer), (value))

#define atomicFetchAnd(pointer, value) _Generic((pointer), \
    unsigned long *: atomicFetchAndUL, \
    unsigned int *: atomicFetchAndU, \
    unsigned short *: atomicFetchAndUS, \
    unsigned char *: atomicFetchAndUC \
)((pointer), (value))

#define atomicFetchOr(pointer, value) _Generic((pointer), \
    unsigned long *: atomicFetchOrUL, \
    unsigned int *: atomicFetchOrU, \
    unsigned short *: atomicFetchOrUS, \
    unsigned char *: atomicFetchOrUC \
)((pointer), (value))

#define atomicFetchSub(pointer, value) _Generic((pointer), \
    unsigned long *: atomicFetchSubUL, \
    unsigned int *: atomicFetchSubU, \
    unsigned short *: atomicFetchSubUS, \
    unsigned char *: atomicFetchSubUC \
)((pointer), (value))
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV6M_ATOMIC_H_ */
