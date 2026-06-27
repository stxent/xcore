/*
 * xcore/crc/crc8_maxim.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CRC_CRC8_MAXIM_H_
#define XCORE_CRC_CRC8_MAXIM_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stddef.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

uint8_t crc8MaximUpdate(uint8_t, const void *, size_t);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CRC_CRC8_MAXIM_H_ */
