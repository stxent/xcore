/*
 * xcore/core/cortex/armv7m/memory.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_CORTEX_ARMV7M_MEMORY_H_
#define XCORE_CORE_CORTEX_ARMV7M_MEMORY_H_
/*----------------------------------------------------------------------------*/
#include "../armv6m/memory.h"
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

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
bool compareExchangePointer(volatile void *pointer, void *expected,
    void *desired);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV7M_MEMORY_H_ */
