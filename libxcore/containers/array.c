/*
 * array.c
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <stdlib.h>
#include <xcore/containers/array.h>
/*----------------------------------------------------------------------------*/
enum result arrayInit(struct Array *array, size_t width, size_t capacity)
{
  if (!capacity)
    return E_VALUE;

  array->data = malloc(width * capacity);
  if (!array->data)
    return E_MEMORY;

  array->capacity = capacity;
  array->size = 0;
  array->width = width;

  return E_OK;
}
/*----------------------------------------------------------------------------*/
void arrayDeinit(struct Array *array)
{
  free(array->data);
}
/*----------------------------------------------------------------------------*/
void arrayErase(struct Array *array, size_t index)
{
  assert(index < array->size);

  const uintptr_t destination = (uintptr_t)array->data + index * array->width;
  const uintptr_t source = destination + array->width;

  --array->size;

  memmove((void *)destination, (const void *)source,
      (array->size - index) * array->width);
}
/*----------------------------------------------------------------------------*/
void arrayInsert(struct Array *array, size_t before, const void *element)
{
  assert(before <= array->size);
  assert(array->size < array->capacity);

  const uintptr_t source = (uintptr_t)array->data + before * array->width;
  const uintptr_t destination = source + array->width;

  memmove((void *)destination, (const void *)source,
      (array->size - before) * array->width);
  memcpy((void *)source, element, array->width);

  ++array->size;
}
/*----------------------------------------------------------------------------*/
void arrayPopBack(struct Array *array, void *element)
{
  assert(array->size > 0);

  if (element)
  {
    memcpy(element, (const void *)((uintptr_t)array->data
        + (array->size - 1) * array->width), array->width);
  }
  --array->size;
}
/*----------------------------------------------------------------------------*/
void arrayPushBack(struct Array *array, const void *element)
{
  assert(array->size < array->capacity);

  memcpy((void *)((uintptr_t)array->data + array->size * array->width),
      element, array->width);
  ++array->size;
}
