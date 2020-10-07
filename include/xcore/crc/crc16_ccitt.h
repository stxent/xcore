/*
 * xcore/crc/crc16_ccitt.h
 * Copyright (C) 2013 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CRC_CRC16_CCITT_H_
#define XCORE_CRC_CRC16_CCITT_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stddef.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

uint16_t crc16CCITTUpdate(uint16_t, const void *, size_t);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC16_CCITT_H_ */
