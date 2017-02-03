/*
 * xcore/crc/crc7.h
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CRC_CRC7_H_
#define XCORE_CRC_CRC7_H_
/*----------------------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
uint8_t crc7Update(uint8_t, const void *, size_t);
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC7_H_ */