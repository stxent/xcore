/*
 * xcore/core/generic/accel_saturated.h
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_ACCEL_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_GENERIC_ACCEL_CLZ_H_
#define XCORE_CORE_GENERIC_ACCEL_CLZ_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stdint.h>
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

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_GENERIC_ACCEL_CLZ_H_ */
