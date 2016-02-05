/*
 * memory.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <memory.h>
/*----------------------------------------------------------------------------*/
bool compareExchangePointer(void **pointer, const void *expected, void *desired)
{
  bool status = false;

  __interruptsDisable();
  /* Compare current state with expected state */
  if (*pointer == expected)
  {
    *pointer = desired;
    status = true;
  }
  __interruptsEnable();

  return status;
}
/*----------------------------------------------------------------------------*/
uint32_t countLeadingZeros32(uint32_t value)
{
  /* If input value is 0, the result is undefined */
  uint32_t count = 1;

  if (!(value & 0xFFFF0000))
  {
    count += 16;
    value <<= 16;
  }
  if (!(value & 0xFF000000))
  {
    count += 8;
    value <<= 8;
  }
  if (!(value & 0xF0000000))
  {
    count += 4;
    value <<= 4;
  }
  if (!(value & 0xC0000000))
  {
    count += 2;
    value <<= 2;
  }
  count -= value >> 31;

  return count;
}
