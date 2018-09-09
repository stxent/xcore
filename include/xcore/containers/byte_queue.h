/*
 * xcore/containers/byte_queue.h
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CONTAINERS_BYTE_QUEUE_H_
#define XCORE_CONTAINERS_BYTE_QUEUE_H_
/*----------------------------------------------------------------------------*/
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <xcore/helpers.h>
/*----------------------------------------------------------------------------*/
struct ByteQueue
{
  uint8_t *data;
  /** Maximum capacity of the queue. */
  size_t capacity;
  /** Current number of elements in the queue. */
  size_t size;
  /** Index of the first element. */
  size_t head;
  /** Index of the last element. */
  size_t tail;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

bool byteQueueInit(struct ByteQueue *, size_t);
void byteQueueDeinit(struct ByteQueue *);
size_t byteQueuePopArray(struct ByteQueue *, void *, size_t);
size_t byteQueuePushArray(struct ByteQueue *, const void *, size_t);

END_DECLS
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline size_t byteQueueCapacity(const struct ByteQueue *queue)
{
  return queue->capacity;
}

static inline void byteQueueClear(struct ByteQueue *queue)
{
  queue->size = 0;
  queue->head = 0;
  queue->tail = 0;
}

static inline bool byteQueueEmpty(const struct ByteQueue *queue)
{
  return queue->size == 0;
}

static inline bool byteQueueFull(const struct ByteQueue *queue)
{
  return queue->size == queue->capacity;
}

static inline uint8_t byteQueueFront(const struct ByteQueue *queue)
{
  assert(queue->size);

  return queue->data[queue->head];
}

static inline uint8_t byteQueuePopFront(struct ByteQueue *queue)
{
  assert(queue->size);

  const uint8_t tmp = queue->data[queue->head++];

  if (queue->head == queue->capacity)
    queue->head = 0;

  --queue->size;

  return tmp;
}

static inline void byteQueuePushBack(struct ByteQueue *queue, uint8_t value)
{
  assert(queue->size < queue->capacity);

  queue->data[queue->tail++] = value;

  if (queue->tail == queue->capacity)
    queue->tail = 0;

  ++queue->size;
}

static inline size_t byteQueueSize(const struct ByteQueue *queue)
{
  return queue->size;
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CONTAINERS_BYTE_QUEUE_H_ */
