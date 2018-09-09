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
bool queueInit(struct Queue *queue, size_t width, size_t capacity)
{
  queue->data = malloc(width * capacity);

  if (queue->data)
  {
    queue->capacity = capacity;
    queue->width = width;
    queueClear(queue);
    return true;
  }
  else
    return false;
}
/*----------------------------------------------------------------------------*/
void queueDeinit(struct Queue *queue)
{
  free(queue->data);
}
/*----------------------------------------------------------------------------*/
void *queueAt(struct Queue *queue, size_t index)
{
  assert(queue->size > index);

  index += queue->head;
  if (index >= queue->capacity)
    index -= queue->capacity;

  return (void *)((uintptr_t)queue->data + index * queue->width);
}
/*----------------------------------------------------------------------------*/
void queueFront(const struct Queue *queue, void *element)
{
  assert(element);
  assert(queue->size > 0);

  const uintptr_t address = (uintptr_t)queue->data
      + queue->head * queue->width;
  memcpy(element, (const void *)address, queue->width);
}
/*----------------------------------------------------------------------------*/
void queuePopFront(struct Queue *queue)
{
  assert(queue->size > 0);

  if (++queue->head == queue->capacity)
    queue->head = 0;
  --queue->size;
}
/*----------------------------------------------------------------------------*/
void queuePushBack(struct Queue *queue, const void *element)
{
  assert(element);
  assert(queue->size < queue->capacity);

  const uintptr_t address = (uintptr_t)queue->data
      + queue->tail * queue->width;
  memcpy((void *)address, element, queue->width);

  if (++queue->tail == queue->capacity)
    queue->tail = 0;
  ++queue->size;
}
