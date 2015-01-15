/*
 * unicode.h
 * Copyright (C) 2013 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef UNICODE_H_
#define UNICODE_H_
/*----------------------------------------------------------------------------*/
#include <stdint.h>
/*----------------------------------------------------------------------------*/
/* Type for Unicode UTF-16 characters */
typedef uint16_t char16_t;
/*----------------------------------------------------------------------------*/
uint16_t uFromUtf16(char *, const char16_t *, uint16_t);
uint16_t uToUtf16(char16_t *, const char *, uint16_t);
/*----------------------------------------------------------------------------*/
#endif /* UNICODE_H_ */
