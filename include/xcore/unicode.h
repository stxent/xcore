/*
 * xcore/unicode.h
 * Copyright (C) 2013 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_UNICODE_H_
#define XCORE_UNICODE_H_
/*----------------------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
/* Type for Unicode UTF-16 characters */
typedef uint16_t char16_t;
/*----------------------------------------------------------------------------*/
size_t uLengthFromUtf16(const char16_t *);
size_t uLengthToUtf16(const char *);
size_t uFromUtf16(char *, const char16_t *, size_t);
size_t uToUtf16(char16_t *, const char *, size_t);
/*----------------------------------------------------------------------------*/
#endif /* XCORE_UNICODE_H_ */
