/*
 * memory.c
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/memory.h>
/*----------------------------------------------------------------------------*/
bool compareExchangePointer(volatile void *pointer, void *expected,
    void *desired)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  bool result;

  if (*(void **)pointer == *(void **)expected)
  {
    *(void **)pointer = desired;
    result = true;
  }
  else
  {
    *(void **)expected = *(void **)pointer;
    result = false;
  }

  __interruptsSetState(state);
  return result;
}
