/*
 * xcore/crc/crc32_ieee.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CRC_CRC32_IEEE_H_
#define XCORE_CRC_CRC32_IEEE_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stddef.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

uint32_t crc32IEEEUpdate(uint32_t, const void *, size_t);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC32_IEEE_H_ */
