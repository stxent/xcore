/*
 * xcore/core/cortex/armv6m/accel.h
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_CORTEX_ARMV6M_ACCEL_H_
#define XCORE_CORE_CORTEX_ARMV6M_ACCEL_H_
/*----------------------------------------------------------------------------*/
#include <xcore/asm.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline uint32_t countLeadingZeros32(uint32_t value)
{
  static uint8_t const table[] = {
      32, 31, 30, 30, 29, 29, 29, 29,
      28, 28, 28, 28, 28, 28, 28, 28
  };
  uint32_t result;

  if (value >= (1U << 16))
  {
    if (value >= (1U << 24))
      result = (value >= (1U << 28)) ? 28 : 24;
    else
      result = (value >= (1U << 20)) ? 20 : 16;
  }
  else
  {
    if (value >= (1U << 8))
      result = (value >= (1U << 12)) ? 12 : 8;
    else
      result = (value >= (1U << 4)) ? 4 : 0;
  }

  return table[value >> result] - result;
}

static inline uint32_t reverseBits32(uint32_t value)
{
  /* Swap odd and even bits */
  value = ((value >> 1) & 0x55555555UL) | ((value & 0x55555555UL) << 1);
  /* Swap consecutive pairs */
  value = ((value >> 2) & 0x33333333UL) | ((value & 0x33333333UL) << 2);
  /* Swap nibbles */
  value = ((value >> 4) & 0x0F0F0F0FUL) | ((value & 0x0F0F0F0FUL) << 4);
  /* Swap bytes and pairs */
  value = __rev(value);

  return value;
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV6M_ACCEL_H_ */
