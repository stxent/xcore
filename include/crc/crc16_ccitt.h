/*
 * crc/crc16_ccitt.h
 * Copyright (C) 2013 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef CRC_CRC16_CCITT_H_
#define CRC_CRC16_CCITT_H_
/*----------------------------------------------------------------------------*/
#include <crc.h>
/*----------------------------------------------------------------------------*/
extern const struct CrcEngineClass * const Crc16Ccitt;
/*----------------------------------------------------------------------------*/
struct Crc16Ccitt
{
  struct CrcEngine base;
};
/*----------------------------------------------------------------------------*/
#endif /* CRC_CRC16_CCITT_H_ */
