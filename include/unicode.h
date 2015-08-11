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
unsigned int uLengthFromUtf16(const char16_t *);
unsigned int uLengthToUtf16(const char *);
unsigned int uFromUtf16(char *, const char16_t *, unsigned int);
unsigned int uToUtf16(char16_t *, const char *, unsigned int);
/*----------------------------------------------------------------------------*/
#endif /* UNICODE_H_ */
