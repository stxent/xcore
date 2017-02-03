/*
 * xcore/crc/crc16_ccitt.h
 * Copyright (C) 2013 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CRC_CRC16_CCITT_H_
#define XCORE_CRC_CRC16_CCITT_H_
/*----------------------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
uint16_t crc16CCITTUpdate(uint16_t, const void *, size_t);
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC16_CCITT_H_ */