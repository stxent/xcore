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
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

bool atomicCompareExchangeUL(unsigned long *, unsigned long *,
    unsigned long);
bool atomicCompareExchangeU(unsigned int *, unsigned int *,
    unsigned int);
bool atomicCompareExchangeUS(unsigned short *, unsigned short *,
    unsigned short);
bool atomicCompareExchangeUC(unsigned char *, unsigned char *,
    unsigned char);

unsigned long atomicFetchAddUL(unsigned long *, unsigned long);
unsigned int atomicFetchAddU(unsigned int *, unsigned int);
unsigned short atomicFetchAddUS(unsigned short *, unsigned short);
unsigned char atomicFetchAddUC(unsigned char *, unsigned char);

unsigned long atomicFetchAndUL(unsigned long *, unsigned long);
unsigned int atomicFetchAndU(unsigned int *, unsigned int);
unsigned short atomicFetchAndUS(unsigned short *, unsigned short);
unsigned char atomicFetchAndUC(unsigned char *, unsigned char);

unsigned long atomicFetchOrUL(unsigned long *, unsigned long);
unsigned int atomicFetchOrU(unsigned int *, unsigned int);
unsigned short atomicFetchOrUS(unsigned short *, unsigned short);
unsigned char atomicFetchOrUC(unsigned char *, unsigned char);

unsigned long atomicFetchSubUL(unsigned long *, unsigned long);
unsigned int atomicFetchSubU(unsigned int *, unsigned int);
unsigned short atomicFetchSubUS(unsigned short *, unsigned short);
unsigned char atomicFetchSubUC(unsigned char *, unsigned char);

unsigned long atomicLoadUL(const unsigned long *);
unsigned int atomicLoadU(const unsigned int *);
unsigned short atomicLoadUS(const unsigned short *);
unsigned char atomicLoadUC(const unsigned char *);

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
bool compareExchangePointer(void *, void *, void *);

END_DECLS
/*----------------------------------------------------------------------------*/
#define atomicCompareExchange(pointer, expected, desired) _Generic((pointer), \
    unsigned long *: atomicCompareExchangeUL, \
    unsigned int *: atomicCompareExchangeU, \
    unsigned short *: atomicCompareExchangeUS, \
    unsigned char *: atomicCompareExchangeUC \
)((pointer), (expected), (desired))

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

#define atomicLoad(pointer) _Generic((pointer), \
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
#endif /* XCORE_CORE_CORTEX_ARMV6M_ATOMIC_H_ */
