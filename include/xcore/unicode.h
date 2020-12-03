/*
 * xcore/unicode.h
 * Copyright (C) 2013 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_UNICODE_H_
#define XCORE_UNICODE_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stddef.h>
#include <stdint.h>
#include <uchar.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

size_t uLengthFromUtf16(const char16_t *);
size_t uLengthToUtf16(const char *);
size_t uFromUtf16(char *, const char16_t *, size_t);
size_t uToUtf16(char16_t *, const char *, size_t);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_UNICODE_H_ */
