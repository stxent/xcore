/*
 * xcore/bits.h
 * Copyright (C) 2013 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_BITS_H_
#define XCORE_BITS_H_
/*----------------------------------------------------------------------------*/
#define BIT(shift)                      (1UL << (shift))
#define BIT_FIELD(value, shift)         ((unsigned long)(value) << (shift))
#define FIELD_VALUE(reg, mask, shift)   (((reg) & (mask)) >> (shift))
#define MASK(width)                     ((1UL << (width)) - 1)
/*----------------------------------------------------------------------------*/
#define ARRAY_SIZE(array)               (sizeof(array) / sizeof(*(array)))
/*----------------------------------------------------------------------------*/
#define TO_STRING(text)                 #text
#define EXPAND_TO_STRING(text)          TO_STRING(text)
/*----------------------------------------------------------------------------*/
#endif /* XCORE_BITS_H_ */