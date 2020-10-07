/*
 * xcore/crc/crc32.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CRC_CRC32_H_
#define XCORE_CRC_CRC32_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stddef.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

uint32_t crc32Update(uint32_t, const void *, size_t);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC32_H_ */
