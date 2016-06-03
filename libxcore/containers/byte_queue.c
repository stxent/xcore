/*
 * byte_queue.c
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <xcore/containers/byte_queue.h>
/*----------------------------------------------------------------------------*/
enum result byteQueueInit(struct ByteQueue *queue, size_t capacity)
{
  if (!capacity)
    return E_VALUE;

  queue->data = malloc(capacity);
  if (!queue->data)
    return E_MEMORY;

  queue->capacity = capacity;
  byteQueueClear(queue);

  return E_OK;
}
/*----------------------------------------------------------------------------*/
void byteQueueDeinit(struct ByteQueue *queue)
{
  free(queue->data);
}
/*----------------------------------------------------------------------------*/
size_t byteQueuePopArray(struct ByteQueue *queue, void *buffer, size_t length)
{
  if (!queue->size)
    return 0;

  uintptr_t bufferPosition = (uintptr_t)buffer;
  size_t bytesToRead = length;

  if (queue->ceil <= queue->floor)
  {
    size_t count = queue->capacity - queue->floor;

    if (bytesToRead < count)
      count = bytesToRead;

    if (count)
    {
      memcpy((void *)bufferPosition, queue->data + queue->floor, count);
      queue->floor += count;
      if (queue->floor == queue->capacity)
        queue->floor = 0;

      bufferPosition += count;
      bytesToRead -= count;
    }
  }

  if (queue->ceil > queue->floor)
  {
    size_t count = queue->ceil - queue->floor;

    if (bytesToRead < count)
      count = bytesToRead;

    if (count)
    {
      memcpy((void *)bufferPosition, queue->data + queue->floor, count);
      queue->floor += count;
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
  if (queue->capacity == queue->size)
    return 0;

  uintptr_t bufferPosition = (uintptr_t)buffer;
  size_t bytesToWrite = length;

  if (queue->ceil >= queue->floor)
  {
    size_t count = queue->capacity - queue->ceil;

    if (bytesToWrite < count)
      count = bytesToWrite;

    if (count)
    {
      memcpy(queue->data + queue->ceil, (const void *)bufferPosition, count);
      queue->ceil = queue->ceil + count;
      if (queue->ceil == queue->capacity)
        queue->ceil = 0;

      bufferPosition += count;
      bytesToWrite -= count;
    }
  }

  if (queue->ceil < queue->floor)
  {
    size_t count = queue->floor - queue->ceil;

    if (bytesToWrite < count)
      count = bytesToWrite;

    if (count)
    {
      memcpy(queue->data + queue->ceil, (const void *)bufferPosition, count);
      queue->ceil += count;
      bytesToWrite -= count;
    }
  }

  const size_t bytesWritten = length - bytesToWrite;

  queue->size += bytesWritten;
  return bytesWritten;
}
