/*
 * crc/crc8dallas.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef CRC_CRC8DALLAS_H_
#define CRC_CRC8DALLAS_H_
/*----------------------------------------------------------------------------*/
#include <crc.h>
/*----------------------------------------------------------------------------*/
extern const struct CrcEngineClass * const Crc8Dallas;
/*----------------------------------------------------------------------------*/
struct Crc8Dallas
{
  struct CrcEngine parent;
};
/*----------------------------------------------------------------------------*/
#endif /* CRC_CRC8DALLAS_H_ */
