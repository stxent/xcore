/*
 * array.c
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/array.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
bool arrayInit(struct Array *array, size_t width, size_t capacity)
{
  array->data = malloc(width * capacity);

  if (array->data)
  {
    array->capacity = capacity;
    array->size = 0;
    array->width = width;
    return true;
  }
  else
    return false;
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
  assert(element);
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
void arrayPopBack(struct Array *array)
{
  assert(array->size > 0);
  --array->size;
}
/*----------------------------------------------------------------------------*/
void arrayPushBack(struct Array *array, const void *element)
{
  assert(element);
  assert(array->size < array->capacity);

  const uintptr_t address = (uintptr_t)array->data + array->size * array->width;
  memcpy((void *)address, element, array->width);

  ++array->size;
}
