/*
 * queue.c
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/queue.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
bool queueInit(struct Queue *queue, size_t width, size_t capacity)
{
  queue->data = malloc(width * capacity);

  if (queue->data != NULL)
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
void queueBack(const struct Queue *queue, void *element)
{
  assert(element != NULL);
  assert(queue->size > 0);

  const size_t index = (queue->tail ? queue->tail : queue->capacity) - 1;
  const uintptr_t address = (uintptr_t)queue->data + index * queue->width;
  memcpy(element, (const void *)address, queue->width);
}
/*----------------------------------------------------------------------------*/
void queueFront(const struct Queue *queue, void *element)
{
  assert(element != NULL);
  assert(queue->size > 0);

  const uintptr_t address = (uintptr_t)queue->data + queue->head * queue->width;
  memcpy(element, (const void *)address, queue->width);
}
/*----------------------------------------------------------------------------*/
void queuePopBack(struct Queue *queue)
{
  assert(queue->size > 0);

  queue->tail = (queue->tail ? queue->tail : queue->capacity) - 1;
  --queue->size;
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
  assert(element != NULL);
  assert(queue->size < queue->capacity);

  const uintptr_t address = (uintptr_t)queue->data + queue->tail * queue->width;
  memcpy((void *)address, element, queue->width);

  if (++queue->tail == queue->capacity)
    queue->tail = 0;
  ++queue->size;
}
/*----------------------------------------------------------------------------*/
void queuePushFront(struct Queue *queue, const void *element)
{
  assert(element != NULL);
  assert(queue->size < queue->capacity);

  const size_t index = (queue->head ? queue->head : queue->capacity) - 1;
  const uintptr_t address = (uintptr_t)queue->data + index * queue->width;
  memcpy((void *)address, element, queue->width);

  queue->head = index;
  ++queue->size;
}
