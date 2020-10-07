/*
 * byte_queue.c
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <xcore/containers/byte_queue.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
bool byteQueueInit(struct ByteQueue *queue, size_t capacity)
{
  queue->data = malloc(capacity);

  if (queue->data)
  {
    queue->capacity = capacity;
    byteQueueClear(queue);
    return true;
  }
  else
    return false;
}
/*----------------------------------------------------------------------------*/
void byteQueueDeinit(struct ByteQueue *queue)
{
  free(queue->data);
}
/*----------------------------------------------------------------------------*/
size_t byteQueuePopArray(struct ByteQueue *queue, void *buffer, size_t length)
{
  assert(buffer);

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
  assert(buffer);

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
