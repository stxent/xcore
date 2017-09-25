/*
 * xcore/helpers.h
 * Copyright (C) 2017 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_HELPERS_H_
#define XCORE_HELPERS_H_
/*----------------------------------------------------------------------------*/
#undef BEGIN_DECLS
#undef END_DECLS

#ifdef __cplusplus

#define BEGIN_DECLS extern "C" {
#define END_DECLS }

#else

#define BEGIN_DECLS /* empty */
#define END_DECLS /* empty */

#endif
/*----------------------------------------------------------------------------*/
#define ARRAY_SIZE(array)       (sizeof(array) / sizeof(*(array)))
/*----------------------------------------------------------------------------*/
/*
 * The classic MIN and MAX macros.
 * Note that statements will be evaluated twice.
 */
#define MAX(a, b)               ((a) > (b) ? (a) : (b))
#define MIN(a, b)               ((a) > (b) ? (b) : (a))
/*----------------------------------------------------------------------------*/
#define TO_STRING(text)         #text
#define EXPAND_TO_STRING(text)  TO_STRING(text)
/*----------------------------------------------------------------------------*/
#endif /* XCORE_HELPERS_H_ */
