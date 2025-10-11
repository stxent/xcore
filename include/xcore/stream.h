/*
 * xcore/stream.h
 * Copyright (C) 2021 xent
 * Project is distributed under the terms of the MIT License
 */

/**
 * @file
 * Abstract interface for peripherals with packet-oriented input or output.
 */

#ifndef XCORE_STREAM_H_
#define XCORE_STREAM_H_
/*----------------------------------------------------------------------------*/
#include <xcore/entity.h>
/*----------------------------------------------------------------------------*/
enum StreamRequestStatus
{
  /** Request completed successfully. */
  STREAM_REQUEST_COMPLETED,
  /** Request is removed from the queue. */
  STREAM_REQUEST_CANCELLED,
  /** Request is not completed. */
  STREAM_REQUEST_FAILED
};

struct StreamRequest;

typedef void (*StreamRequestCallback)(void *, struct StreamRequest *,
    enum StreamRequestStatus);

struct StreamRequest
{
  size_t capacity;
  size_t length;

  StreamRequestCallback callback;
  void *argument;

  void *buffer;
};

struct StreamClass
{
  CLASS_HEADER

  void (*clear)(void *);
  enum Result (*enqueue)(void *, struct StreamRequest *);
};

struct Stream
{
  struct Entity base;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

/**
 * @brief Clears the descriptor queue.
 * @param[in] stream Pointer to a Stream object.
 */
static inline void streamClear(void *stream)
{
  ((const struct StreamClass *)CLASS(stream))->clear(stream);
}

/**
 * @brief Adds a descriptor to the queue.
 * @param[in] stream Pointer to a Stream object.
 * @param[in] request Pointer to the StreamDescriptor object to be added.
 * @return @b E_OK if the operation was successful.
 */
static inline enum Result streamEnqueue(void *stream,
    struct StreamRequest *request)
{
  return ((const struct StreamClass *)CLASS(stream))->enqueue(stream, request);
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_STREAM_H_ */
