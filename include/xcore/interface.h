/*
 * xcore/interface.h
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the MIT License
 */

/**
 * @file
 * Abstract interface for peripherals with input and output capability.
 */

#ifndef XCORE_INTERFACE_H_
#define XCORE_INTERFACE_H_
/*----------------------------------------------------------------------------*/
#include <xcore/entity.h>
/*----------------------------------------------------------------------------*/
/** Interface options. */
enum IfParameter
{
  /** Elements available in the receive queue. */
  IF_AVAILABLE,
  /** Elements pending in the transmit queue. */
  IF_PENDING,

  /** Unique identifier of the device, 32-bit value. */
  IF_ADDRESS,
  /** Unique identifier of the device, 64-bit value. */
  IF_ADDRESS_64,
  /** Priority of interrupts or direct memory access requests. */
  IF_PRIORITY,
  /** Data rate. */
  IF_RATE,

  /** Alignment of the buffer length for user space buffers. */
  IF_ALIGNMENT,
  /** Position in the 32-bit address space. */
  IF_POSITION,
  /** Position in the 64-bit address space. */
  IF_POSITION_64,
  /** Size of the available space in a 32-bit address space. */
  IF_SIZE,
  /** Size of the available space in a 64-bit address space. */
  IF_SIZE_64,
  /** Size of a data word in bits. */
  IF_WIDTH,

  /**
   * Get execution status of the last command. Returns @b E_OK when interface
   * is ready to accept a new command. Data pointer should be set to zero.
   */
  IF_STATUS,

  /**
   * Select blocking mode for the interface. Data pointer should be set
   * to zero.
   */
  IF_BLOCKING,
  /**
   * Select non-blocking mode for the interface. Data pointer should be set
   * to zero.
   */
  IF_NONBLOCKING,
  /**
   * Select zero-copy mode for the interface. Data pointer should be set
   * to zero. This mode is similar to the non-blocking mode except for
   * user space buffers being unavailable until the transfer completion.
   */
  IF_ZEROCOPY,

  /**
   * Acquire the interface. Returns @b E_OK on success or @b E_BUSY.
   * Data pointer should be set to zero.
   */
  IF_ACQUIRE,
  /** Release the interface. Data pointer should be set to zero. */
  IF_RELEASE,

  /** End of the list. */
  IF_PARAMETER_END
};
/*----------------------------------------------------------------------------*/
struct InterfaceClass
{
  CLASS_HEADER

  void (*setCallback)(void *, void (*)(void *), void *);
  enum Result (*getParam)(void *, int, void *);
  enum Result (*setParam)(void *, int, const void *);
  size_t (*read)(void *, void *, size_t);
  size_t (*write)(void *, const void *, size_t);
};

struct Interface
{
  struct Entity base;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

/**
 * Set callback function and its argument called on completion event.
 * @param interface Pointer to an Interface object.
 * @param callback Callback function.
 * @param argument Callback function argument.
 */
static inline void ifSetCallback(void *interface, void (*callback)(void *),
    void *argument)
{
  ((const struct InterfaceClass *)CLASS(interface))->setCallback(interface,
      callback, argument);
}

/**
 * Read the interface parameter.
 * @param interface Pointer to an Interface object.
 * @param parameter Parameter to be read.
 * @param data Pointer to a variable where a value of the option will be stored.
 * @return @b E_OK on success.
 */
static inline enum Result ifGetParam(void *interface, int parameter, void *data)
{
  return ((const struct InterfaceClass *)CLASS(interface))->getParam(interface,
      parameter, data);
}

/**
 * Set the interface parameter.
 * @param interface Pointer to an Interface object.
 * @param parameter Parameter to be set.
 * @param data Pointer to a new value of the option.
 * @return @b E_OK on success.
 */
static inline enum Result ifSetParam(void *interface, int parameter,
    const void *data)
{
  return ((const struct InterfaceClass *)CLASS(interface))->setParam(interface,
      parameter, data);
}

/**
 * Receive data from the interface.
 * @param interface Pointer to an Interface object.
 * @param buffer Pointer to a buffer with a size of at least @b length bytes.
 * @param length Number of bytes to be read.
 * @return The total number of elements successfully read is returned.
 */
static inline size_t ifRead(void *interface, void *buffer, size_t length)
{
  return ((const struct InterfaceClass *)CLASS(interface))->read(interface,
      buffer, length);
}

/**
 * Send data over the interface.
 * @param interface Pointer to an Interface object.
 * @param buffer Pointer to a buffer with a size of at least @b length bytes.
 * @param length Number of bytes to be written.
 * @return The total number of elements successfully written is returned.
 */
static inline size_t ifWrite(void *interface, const void *buffer, size_t length)
{
  return ((const struct InterfaceClass *)CLASS(interface))->write(interface,
      buffer, length);
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_INTERFACE_H_ */
