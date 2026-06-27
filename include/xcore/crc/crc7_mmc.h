/*
 * xcore/crc/crc7_mmc.h
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CRC_CRC7_MMC_H_
#define XCORE_CRC_CRC7_MMC_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stddef.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

uint8_t crc7MMCUpdate(uint8_t, const void *, size_t);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC7_MMC_H_ */
