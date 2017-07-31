/*
 * xcore/core/cortex/m4/memory.h
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_CORTEX_M4_MEMORY_H_
#define XCORE_CORE_CORTEX_M4_MEMORY_H_
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <xcore/core/cortex/m4/asm.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline uint32_t countLeadingZeros32(uint32_t value)
{
  return __clz(value);
}
/*----------------------------------------------------------------------------*/
static inline uint32_t reverseBits32(uint32_t value)
{
  return __rbit(value);
}
/*----------------------------------------------------------------------------*/
static inline uint64_t toBigEndian64(uint64_t value)
{
  return (uint64_t)__rev(value) << 32 | (uint64_t)__rev(value >> 32);
}
/*----------------------------------------------------------------------------*/
static inline uint32_t toBigEndian32(uint32_t value)
{
  return __rev(value);
}
/*----------------------------------------------------------------------------*/
static inline uint16_t toBigEndian16(uint16_t value)
{
  return __rev16(value);
}
/*----------------------------------------------------------------------------*/
static inline uint64_t toLittleEndian64(uint64_t value)
{
  return value;
}
/*----------------------------------------------------------------------------*/
static inline uint32_t toLittleEndian32(uint32_t value)
{
  return value;
}
/*----------------------------------------------------------------------------*/
static inline uint16_t toLittleEndian16(uint16_t value)
{
  return value;
}
/*----------------------------------------------------------------------------*/
static inline uint64_t fromBigEndian64(uint64_t value)
{
  return toBigEndian64(value);
}
/*----------------------------------------------------------------------------*/
static inline uint32_t fromBigEndian32(uint32_t value)
{
  return toBigEndian32(value);
}
/*----------------------------------------------------------------------------*/
static inline uint16_t fromBigEndian16(uint16_t value)
{
  return toBigEndian16(value);
}
/*----------------------------------------------------------------------------*/
static inline uint64_t fromLittleEndian64(uint64_t value)
{
  return value;
}
/*----------------------------------------------------------------------------*/
static inline uint32_t fromLittleEndian32(uint32_t value)
{
  return value;
}
/*----------------------------------------------------------------------------*/
static inline uint16_t fromLittleEndian16(uint16_t value)
{
  return value;
}

END_DECLS
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

bool compareExchangePointer(void **, const void *, void *);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_M4_MEMORY_H_ */
