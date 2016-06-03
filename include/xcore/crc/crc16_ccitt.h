/*
 * xcore/crc/crc16_ccitt.h
 * Copyright (C) 2013 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CRC_CRC16_CCITT_H_
#define XCORE_CRC_CRC16_CCITT_H_
/*----------------------------------------------------------------------------*/
#include <xcore/crc.h>
/*----------------------------------------------------------------------------*/
extern const struct CrcEngineClass * const Crc16CCITT;
/*----------------------------------------------------------------------------*/
struct Crc16CCITT
{
  struct CrcEngine base;
};
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC16_CCITT_H_ */
