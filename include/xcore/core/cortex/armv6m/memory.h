/*
 * xcore/core/cortex/armv6m/memory.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_CORTEX_ARMV6M_MEMORY_H_
#define XCORE_CORE_CORTEX_ARMV6M_MEMORY_H_
/*----------------------------------------------------------------------------*/
#include <xcore/asm.h>
#include <stdbool.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline uint64_t toBigEndian64(uint64_t value)
{
  const uint32_t high = value >> 32;
  const uint32_t low = value;

  return ((uint64_t)__rev(low) << 32) | (uint64_t)__rev(high);
}

static inline uint32_t toBigEndian32(uint32_t value)
{
  return __rev(value);
}

static inline uint16_t toBigEndian16(uint16_t value)
{
  return (uint16_t)__rev16((uint32_t)value);
}

static inline uint64_t toLittleEndian64(uint64_t value)
{
  return value;
}

static inline uint32_t toLittleEndian32(uint32_t value)
{
  return value;
}

static inline uint16_t toLittleEndian16(uint16_t value)
{
  return value;
}

static inline uint64_t fromBigEndian64(uint64_t value)
{
  return toBigEndian64(value);
}

static inline uint32_t fromBigEndian32(uint32_t value)
{
  return toBigEndian32(value);
}

static inline uint16_t fromBigEndian16(uint16_t value)
{
  return toBigEndian16(value);
}

static inline uint64_t fromLittleEndian64(uint64_t value)
{
  return value;
}

static inline uint32_t fromLittleEndian32(uint32_t value)
{
  return value;
}

static inline uint16_t fromLittleEndian16(uint16_t value)
{
  return value;
}

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
#endif /* XCORE_CORE_CORTEX_ARMV6M_MEMORY_H_ */
