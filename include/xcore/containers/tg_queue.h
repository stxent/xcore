/*
 * xcore/containers/tg_queue.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CONTAINERS_TG_QUEUE_H_
#define XCORE_CONTAINERS_TG_QUEUE_H_
/*----------------------------------------------------------------------------*/
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#define DEFINE_QUEUE(type, name, prefix) \
    typedef struct \
    { \
      type *data; \
      size_t capacity; \
      size_t size; \
      size_t head; \
      size_t tail; \
    } name##Queue; \
    \
    [[maybe_unused]] static inline type *prefix##QueueAt( \
        name##Queue *queue, size_t index) \
    { \
      assert(queue->size > index); \
      \
      index += queue->head; \
      if (index >= queue->capacity) \
        index -= queue->capacity; \
      \
      return queue->data + index; \
    } \
    \
    [[maybe_unused]] static inline size_t prefix##QueueCapacity( \
        const name##Queue *queue) \
    { \
      return queue->capacity; \
    } \
    \
    [[maybe_unused]] static inline void prefix##QueueClear( \
        name##Queue *queue) \
    { \
      queue->head = queue->tail = queue->size = 0; \
    } \
    \
    [[maybe_unused]] static inline bool prefix##QueueEmpty( \
        const name##Queue *queue) \
    { \
      return queue->size == 0; \
    } \
    \
    [[maybe_unused]] static inline bool prefix##QueueFull( \
        const name##Queue *queue) \
    { \
      return queue->size == queue->capacity; \
    } \
    \
    [[maybe_unused]] static inline type prefix##QueueBack( \
        const name##Queue *queue) \
    { \
      assert(queue->size > 0); \
      return queue->data[(queue->tail ? queue->tail : queue->capacity) - 1]; \
    } \
    \
    [[maybe_unused]] static inline type prefix##QueueFront( \
        const name##Queue *queue) \
    { \
      assert(queue->size > 0); \
      return queue->data[queue->head]; \
    } \
    \
    [[maybe_unused]] static inline void prefix##QueuePopBack( \
        name##Queue *queue) \
    { \
      assert(queue->size > 0); \
      \
      queue->tail = (queue->tail ? queue->tail : queue->capacity) - 1; \
      --queue->size; \
    } \
    \
    [[maybe_unused]] static inline void prefix##QueuePopFront( \
        name##Queue *queue) \
    { \
      assert(queue->size > 0); \
      \
      if (++queue->head == queue->capacity) \
        queue->head = 0; \
      --queue->size; \
    } \
    \
    [[maybe_unused]] static inline void prefix##QueuePushBack( \
        name##Queue *queue, type element) \
    { \
      assert(queue->size < queue->capacity); \
      \
      queue->data[queue->tail] = element; \
      if (++queue->tail == queue->capacity) \
        queue->tail = 0; \
      ++queue->size; \
    } \
    \
    [[maybe_unused]] static inline void prefix##QueuePushFront( \
        name##Queue *queue, type element) \
    { \
      assert(queue->size < queue->capacity); \
      \
      queue->head = (queue->head ? queue->head : queue->capacity) - 1; \
      queue->data[queue->head] = element; \
      ++queue->size; \
    } \
    \
    [[maybe_unused]] static inline size_t prefix##QueueSize( \
        const name##Queue *queue) \
    { \
      return queue->size; \
    } \
    \
    [[maybe_unused]] static inline bool prefix##QueueInit( \
        name##Queue *queue, size_t capacity) \
    { \
      queue->data = malloc(capacity * sizeof(type)); \
      \
      if (queue->data != NULL) \
      { \
        queue->capacity = capacity; \
        prefix##QueueClear(queue); \
        return true; \
      } \
      else \
      { \
        return false; \
      } \
    } \
    \
    [[maybe_unused]] static inline void prefix##QueueInitArena( \
        name##Queue *queue, size_t capacity, void *arena) \
    { \
      queue->data = arena; \
      queue->capacity = capacity; \
      prefix##QueueClear(queue); \
    } \
    \
    [[maybe_unused]] static inline void prefix##QueueDeinit( \
        name##Queue *queue) \
    { \
      free(queue->data); \
    } \
    \
    [[maybe_unused]] static inline void prefix##QueueDeinitArena( \
        name##Queue *) \
    { \
    }
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CONTAINERS_TG_QUEUE_H_ */
