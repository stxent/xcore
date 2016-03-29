/*
 * containers/queue.h
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CONTAINERS_QUEUE_H_
#define XCORE_CONTAINERS_QUEUE_H_
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stddef.h>
#include <error.h>
/*----------------------------------------------------------------------------*/
struct Queue
{
  void *data;

  /** Maximum capacity of the queue. */
  size_t capacity;
  /** Current number of elements in the queue. */
  size_t size;
  /** Index of the last element. */
  size_t ceil;
  /** Index of the first element. */
  size_t floor;
  /** Size of each element in bytes. */
  size_t width;
};
/*----------------------------------------------------------------------------*/
enum result queueInit(struct Queue *, size_t, size_t);
void queueDeinit(struct Queue *);
void queuePeek(const struct Queue *, void *);
void queuePop(struct Queue *, void *);
void queuePush(struct Queue *, const void *);
/*----------------------------------------------------------------------------*/
static inline size_t queueCapacity(const struct Queue *queue)
{
  return queue->capacity;
}
/*----------------------------------------------------------------------------*/
static inline void queueClear(struct Queue *queue)
{
  queue->floor = queue->ceil = queue->size = 0;
}
/*----------------------------------------------------------------------------*/
static inline bool queueEmpty(const struct Queue *queue)
{
  return queue->size == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool queueFull(const struct Queue *queue)
{
  return queue->size == queue->capacity;
}
/*----------------------------------------------------------------------------*/
static inline size_t queueSize(const struct Queue *queue)
{
  return queue->size;
}
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CONTAINERS_QUEUE_H_ */
