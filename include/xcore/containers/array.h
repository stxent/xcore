/*
 * xcore/containers/array.h
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CONTAINERS_ARRAY_H_
#define XCORE_CONTAINERS_ARRAY_H_
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <xcore/helpers.h>
#include <xcore/error.h>
/*----------------------------------------------------------------------------*/
struct Array
{
  void *data;

  /** Maximum capacity of the vector. */
  size_t capacity;
  /** Current number of elements in the vector. */
  size_t size;
  /** Size in bytes of each element. */
  size_t width;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

enum Result arrayInit(struct Array *, size_t, size_t);
void arrayDeinit(struct Array *);
void arrayErase(struct Array *, size_t);
void arrayInsert(struct Array *, size_t, const void *);
void arrayPopBack(struct Array *, void *);
void arrayPushBack(struct Array *, const void *);

END_DECLS
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline void *arrayAt(const struct Array *array, size_t index)
{
  return (void *)((uintptr_t)array->data + index * array->width);
}

static inline size_t arrayCapacity(const struct Array *array)
{
  return array->capacity;
}

static inline void arrayClear(struct Array *array)
{
  array->size = 0;
}

static inline bool arrayEmpty(const struct Array *array)
{
  return array->size == 0;
}

static inline bool arrayFull(const struct Array *array)
{
  return array->size == array->capacity;
}

static inline size_t arraySize(const struct Array *array)
{
  return array->size;
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CONTAINERS_ARRAY_H_ */
