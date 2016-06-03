/*
 * xcore/crc/crc8_dallas.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CRC_CRC8_DALLAS_H_
#define XCORE_CRC_CRC8_DALLAS_H_
/*----------------------------------------------------------------------------*/
#include <xcore/crc.h>
/*----------------------------------------------------------------------------*/
extern const struct CrcEngineClass * const Crc8Dallas;
/*----------------------------------------------------------------------------*/
struct Crc8Dallas
{
  struct CrcEngine base;
};
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC8_DALLAS_H_ */
