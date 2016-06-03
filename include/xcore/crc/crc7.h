/*
 * xcore/crc/crc7.h
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CRC_CRC7_H_
#define XCORE_CRC_CRC7_H_
/*----------------------------------------------------------------------------*/
#include <xcore/crc.h>
/*----------------------------------------------------------------------------*/
extern const struct CrcEngineClass * const Crc7;
/*----------------------------------------------------------------------------*/
struct Crc7
{
  struct CrcEngine base;
};
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC7_H_ */
