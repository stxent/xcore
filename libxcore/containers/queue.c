/*
 * queue.c
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <xcore/containers/queue.h>
/*----------------------------------------------------------------------------*/
enum Result queueInit(struct Queue *queue, size_t width, size_t capacity)
{
  if (!capacity)
    return E_VALUE;

  queue->data = malloc(width * capacity);
  if (!queue->data)
    return E_MEMORY;

  queue->capacity = capacity;
  queue->width = width;
  queueClear(queue);

  return E_OK;
}
/*----------------------------------------------------------------------------*/
void queueDeinit(struct Queue *queue)
{
  free(queue->data);
}
/*----------------------------------------------------------------------------*/
void queuePeek(const struct Queue *queue, void *element)
{
  assert(queue->size > 0);

  const uintptr_t position =
      (uintptr_t)queue->data + queue->width * queue->floor;

  memcpy(element, (const void *)position, queue->width);
}
/*----------------------------------------------------------------------------*/
void queuePop(struct Queue *queue, void *element)
{
  assert(queue->size > 0);

  if (element)
  {
    const uintptr_t position =
        (uintptr_t)queue->data + queue->width * queue->floor;

    memcpy(element, (const void *)position, queue->width);
  }

  if (++queue->floor == queue->capacity)
    queue->floor = 0;
  --queue->size;
}
/*----------------------------------------------------------------------------*/
void queuePush(struct Queue *queue, const void *element)
{
  assert(queue->size < queue->capacity);

  const uintptr_t position =
      (uintptr_t)queue->data + queue->width * queue->ceil;

  memcpy((void *)position, element, queue->width);

  if (++queue->ceil == queue->capacity)
    queue->ceil = 0;
  ++queue->size;
}
