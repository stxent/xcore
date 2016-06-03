/*
 * vector.c
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <stdlib.h>
#include <xcore/containers/vector.h>
/*----------------------------------------------------------------------------*/
enum result vectorInit(struct Vector *vector, size_t width, size_t capacity)
{
  if (!capacity)
    return E_VALUE;

  vector->data = malloc(width * capacity);
  if (!vector->data)
    return E_MEMORY;

  vector->capacity = capacity;
  vector->size = 0;
  vector->width = width;

  return E_OK;
}
/*----------------------------------------------------------------------------*/
void vectorDeinit(struct Vector *vector)
{
  free(vector->data);
}
/*----------------------------------------------------------------------------*/
void vectorErase(struct Vector *vector, size_t index)
{
  assert(index < vector->size);

  const uintptr_t destination = (uintptr_t)vector->data + index * vector->width;
  const uintptr_t source = destination + vector->width;

  --vector->size;

  memmove((void *)destination, (const void *)source,
      (vector->size - index) * vector->width);
}
/*----------------------------------------------------------------------------*/
void vectorInsert(struct Vector *vector, size_t before, const void *element)
{
  assert(before <= vector->size);
  assert(vector->size < vector->capacity);

  const uintptr_t source = (uintptr_t)vector->data + before * vector->width;
  const uintptr_t destination = source + vector->width;

  memmove((void *)destination, (const void *)source,
      (vector->size - before) * vector->width);
  memcpy((void *)source, element, vector->width);

  ++vector->size;
}
/*----------------------------------------------------------------------------*/
void vectorPopBack(struct Vector *vector, void *element)
{
  assert(vector->size > 0);

  if (element)
  {
    memcpy(element, (const void *)((uintptr_t)vector->data
        + (vector->size - 1) * vector->width), vector->width);
  }
  --vector->size;
}
/*----------------------------------------------------------------------------*/
void vectorPushBack(struct Vector *vector, const void *element)
{
  assert(vector->size < vector->capacity);

  memcpy((void *)((uintptr_t)vector->data + vector->size * vector->width),
      element, vector->width);
  ++vector->size;
}
