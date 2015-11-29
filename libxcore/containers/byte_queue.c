/*
 * byte_queue.c
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <containers/byte_queue.h>
/*----------------------------------------------------------------------------*/
enum result byteQueueInit(struct ByteQueue *queue, unsigned int capacity)
{
  if (!capacity || capacity > USHRT_MAX)
    return E_VALUE;

  queue->data = malloc(capacity);
  if (!queue->data)
    return E_MEMORY;

  queue->capacity = (unsigned short)capacity;
  byteQueueClear(queue);

  return E_OK;
}
/*----------------------------------------------------------------------------*/
void byteQueueDeinit(struct ByteQueue *queue)
{
  free(queue->data);
}
/*----------------------------------------------------------------------------*/
unsigned int byteQueuePopArray(struct ByteQueue *queue, uint8_t *buffer,
    unsigned int length)
{
  if (!queue->size)
    return 0;

  unsigned short bytesToRead = (unsigned short)length;

  if (queue->ceil <= queue->floor)
  {
    unsigned short count = queue->capacity - queue->floor;

    if (bytesToRead < count)
      count = bytesToRead;

    if (count)
    {
      memcpy(buffer, queue->data + queue->floor, count);
      queue->floor += count;
      if (queue->floor == queue->capacity)
        queue->floor = 0;

      buffer += count;
      bytesToRead -= count;
    }
  }

  if (queue->ceil > queue->floor)
  {
    unsigned short count = queue->ceil - queue->floor;

    if (bytesToRead < count)
      count = bytesToRead;

    if (count)
    {
      memcpy(buffer, queue->data + queue->floor, count);
      queue->floor += count;
      bytesToRead -= count;
    }
  }

  const unsigned short bytesRead = length - bytesToRead;

  queue->size -= bytesRead;
  return (unsigned int)bytesRead;
}
/*----------------------------------------------------------------------------*/
unsigned int byteQueuePushArray(struct ByteQueue *queue, const uint8_t *buffer,
    unsigned int length)
{
  if (queue->capacity == queue->size)
    return 0;

  unsigned short bytesToWrite = (unsigned short)length;

  if (queue->ceil >= queue->floor)
  {
    unsigned short count = queue->capacity - queue->ceil;

    if (bytesToWrite < count)
      count = bytesToWrite;

    if (count)
    {
      memcpy(queue->data + queue->ceil, buffer, count);
      queue->ceil = queue->ceil + count;
      if (queue->ceil == queue->capacity)
        queue->ceil = 0;

      buffer += count;
      bytesToWrite -= count;
    }
  }

  if (queue->ceil < queue->floor)
  {
    unsigned short count = queue->floor - queue->ceil;

    if (bytesToWrite < count)
      count = bytesToWrite;

    if (count)
    {
      memcpy(queue->data + queue->ceil, buffer, count);
      queue->ceil += count;
      bytesToWrite -= count;
    }
  }

  const unsigned short bytesWritten = length - bytesToWrite;

  queue->size += bytesWritten;
  return (unsigned int)bytesWritten;
}
