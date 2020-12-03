/*
 * xcore/containers/queue.h
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CONTAINERS_QUEUE_H_
#define XCORE_CONTAINERS_QUEUE_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stdbool.h>
#include <stddef.h>
/*----------------------------------------------------------------------------*/
struct Queue
{
  void *data;

  /** Maximum capacity of the queue. */
  size_t capacity;
  /** Current number of elements in the queue. */
  size_t size;
  /** Index of the first element. */
  size_t head;
  /** Index of the last element. */
  size_t tail;
  /** Size in bytes of each element. */
  size_t width;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

bool queueInit(struct Queue *, size_t, size_t);
void queueDeinit(struct Queue *);
void *queueAt(struct Queue *, size_t);
void queueFront(const struct Queue *, void *);
void queuePopFront(struct Queue *);
void queuePushBack(struct Queue *, const void *);

END_DECLS
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline size_t queueCapacity(const struct Queue *queue)
{
  return queue->capacity;
}

static inline void queueClear(struct Queue *queue)
{
  queue->head = queue->tail = queue->size = 0;
}

static inline bool queueEmpty(const struct Queue *queue)
{
  return queue->size == 0;
}

static inline bool queueFull(const struct Queue *queue)
{
  return queue->size == queue->capacity;
}

static inline size_t queueSize(const struct Queue *queue)
{
  return queue->size;
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CONTAINERS_QUEUE_H_ */
