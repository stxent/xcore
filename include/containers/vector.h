/*
 * containers/vector.h
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CONTAINERS_VECTOR_H_
#define XCORE_CONTAINERS_VECTOR_H_
/*----------------------------------------------------------------------------*/
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <error.h>
/*----------------------------------------------------------------------------*/
struct Vector
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
enum result vectorInit(struct Vector *, size_t, size_t);
void vectorDeinit(struct Vector *);
void vectorErase(struct Vector *, size_t);
void vectorInsert(struct Vector *, size_t, const void *);
void vectorPopBack(struct Vector *, void *);
void vectorPushBack(struct Vector *, const void *);
/*----------------------------------------------------------------------------*/
static inline void *vectorAt(const struct Vector *vector, size_t index)
{
  return (void *)((uintptr_t)vector->data + index * vector->width);
}
/*----------------------------------------------------------------------------*/
static inline size_t vectorCapacity(const struct Vector *vector)
{
  return vector->capacity;
}
/*----------------------------------------------------------------------------*/
static inline void vectorClear(struct Vector *vector)
{
  vector->size = 0;
}
/*----------------------------------------------------------------------------*/
static inline bool vectorEmpty(const struct Vector *vector)
{
  return vector->size == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool vectorFull(const struct Vector *vector)
{
  return vector->size == vector->capacity;
}
/*----------------------------------------------------------------------------*/
static inline size_t vectorSize(const struct Vector *vector)
{
  return vector->size;
}
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CONTAINERS_VECTOR_H_ */
