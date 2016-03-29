/*
 * crc/crc32.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CRC_CRC32_H_
#define XCORE_CRC_CRC32_H_
/*----------------------------------------------------------------------------*/
#include <crc.h>
/*----------------------------------------------------------------------------*/
extern const struct CrcEngineClass * const Crc32;
/*----------------------------------------------------------------------------*/
struct Crc32
{
  struct CrcEngine base;
};
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC32_H_ */
