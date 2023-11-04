/*
 * byte_queue.c
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/byte_queue.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
bool byteQueueInit(struct ByteQueue *queue, size_t capacity)
{
  queue->data = malloc(capacity);

  if (queue->data != NULL)
  {
    queue->capacity = capacity;
    byteQueueClear(queue);
    return true;
  }
  else
    return false;
}
/*----------------------------------------------------------------------------*/
void byteQueueInitArena(struct ByteQueue *queue, size_t capacity, void *arena)
{
  queue->data = arena;
  queue->capacity = capacity;
  byteQueueClear(queue);
}
/*----------------------------------------------------------------------------*/
void byteQueueDeinit(struct ByteQueue *queue)
{
  free(queue->data);
}
/*----------------------------------------------------------------------------*/
void byteQueueDeinitArena(struct ByteQueue *queue __attribute__((unused)))
{
}
/*----------------------------------------------------------------------------*/
size_t byteQueuePopArray(struct ByteQueue *queue, void *buffer, size_t length)
{
  assert(buffer != NULL);

  if (!queue->size)
    return 0;

  uintptr_t bufferPosition = (uintptr_t)buffer;
  size_t bytesToRead = length;

  if (queue->tail <= queue->head)
  {
    size_t count = queue->capacity - queue->head;

    if (bytesToRead < count)
      count = bytesToRead;

    if (count)
    {
      memcpy((void *)bufferPosition, queue->data + queue->head, count);
      queue->head += count;
      if (queue->head == queue->capacity)
        queue->head = 0;

      bufferPosition += count;
      bytesToRead -= count;
    }
  }

  if (queue->tail > queue->head)
  {
    size_t count = queue->tail - queue->head;

    if (bytesToRead < count)
      count = bytesToRead;

    if (count)
    {
      memcpy((void *)bufferPosition, queue->data + queue->head, count);
      queue->head += count;
      bytesToRead -= count;
    }
  }

  const size_t bytesRead = length - bytesToRead;
  queue->size -= bytesRead;
  return bytesRead;
}
/*----------------------------------------------------------------------------*/
size_t byteQueuePushArray(struct ByteQueue *queue, const void *buffer,
    size_t length)
{
  assert(buffer != NULL);

  if (queue->capacity == queue->size)
    return 0;

  uintptr_t bufferPosition = (uintptr_t)buffer;
  size_t bytesToWrite = length;

  if (queue->tail >= queue->head)
  {
    size_t count = queue->capacity - queue->tail;

    if (bytesToWrite < count)
      count = bytesToWrite;

    if (count)
    {
      memcpy(queue->data + queue->tail, (const void *)bufferPosition, count);
      queue->tail = queue->tail + count;
      if (queue->tail == queue->capacity)
        queue->tail = 0;

      bufferPosition += count;
      bytesToWrite -= count;
    }
  }

  if (queue->tail < queue->head)
  {
    size_t count = queue->head - queue->tail;

    if (bytesToWrite < count)
      count = bytesToWrite;

    if (count)
    {
      memcpy(queue->data + queue->tail, (const void *)bufferPosition, count);
      queue->tail += count;
      bytesToWrite -= count;
    }
  }

  const size_t bytesWritten = length - bytesToWrite;
  queue->size += bytesWritten;
  return bytesWritten;
}
/*----------------------------------------------------------------------------*/
void byteQueueAbandon(struct ByteQueue *queue, size_t size)
{
  assert(queue->size >= size);

  queue->head += size;
  if (queue->head >= queue->capacity)
    queue->head -= queue->capacity;
  queue->size -= size;
}
/*----------------------------------------------------------------------------*/
void byteQueueAdvance(struct ByteQueue *queue, size_t size)
{
  assert(queue->size + size <= queue->capacity);

  queue->tail += size;
  if (queue->tail >= queue->capacity)
    queue->tail -= queue->capacity;
  queue->size += size;
}
/*----------------------------------------------------------------------------*/
void byteQueueDeferredPop(struct ByteQueue *queue, const uint8_t **buffer,
    size_t *size, size_t offset)
{
  assert(queue->size >= offset);

  size_t head = queue->head + offset;
  if (head >= queue->capacity)
    head -= queue->capacity;

  *buffer = &queue->data[head];

  if (queue->size > 0)
  {
    if (head < queue->tail)
      *size = queue->size;
    else
      *size = queue->capacity - head;
  }
  else
    *size = 0;
}
/*----------------------------------------------------------------------------*/
void byteQueueDeferredPush(struct ByteQueue *queue, uint8_t **buffer,
    size_t *size, size_t offset)
{
  assert(queue->size + offset <= queue->capacity);

  size_t tail = queue->tail + offset;
  if (tail >= queue->capacity)
    tail -= queue->capacity;

  *buffer = &queue->data[tail];

  if (queue->size < queue->capacity)
  {
    if (queue->head > tail)
      *size = queue->capacity - (queue->size + offset);
    else
      *size = queue->capacity - tail;
  }
  else
    *size = 0;
}
