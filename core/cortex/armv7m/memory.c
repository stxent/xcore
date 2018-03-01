/*
 * memory.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <xcore/memory.h>
/*----------------------------------------------------------------------------*/
bool compareExchangePointer(volatile void *pointer, void *expected,
    void *desired)
{
  return __atomic_compare_exchange((void * volatile *)pointer,
      (void **)expected, &desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}
