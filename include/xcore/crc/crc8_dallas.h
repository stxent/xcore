/*
 * xcore/crc/crc8_dallas.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CRC_CRC8_DALLAS_H_
#define XCORE_CRC_CRC8_DALLAS_H_
/*----------------------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
uint8_t crc8DallasUpdate(uint8_t, const void *, size_t);
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC8_DALLAS_H_ */
