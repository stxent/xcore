/*
 * xcore/core/riscv/memory.h
 * Copyright (C) 2025 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_MEMORY_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_RISCV_MEMORY_H_
#define XCORE_CORE_RISCV_MEMORY_H_
/*----------------------------------------------------------------------------*/
#include <xcore/asm.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline uint64_t toBigEndian64(uint64_t value)
{
  return __builtin_bswap64(value);
}

static inline uint32_t toBigEndian32(uint32_t value)
{
  return __builtin_bswap32(value);
}

static inline uint16_t toBigEndian16(uint16_t value)
{
  return __builtin_bswap16(value);
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

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_RISCV_MEMORY_H_ */
