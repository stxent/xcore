/*
 * interface.h
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

/**
 * @file
 * Abstract interface for peripherals with input and output capability.
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include <entity.h>
#include <error.h>
/*----------------------------------------------------------------------------*/
/** Interface options. */
enum ifOption
{
  /** Bytes available in the receive buffer. */
  IF_AVAILABLE,
  /** Bytes pending in the transmit buffer. */
  IF_PENDING,

  /** The number of available input buffers in the scatter-gather chain. */
  IF_RX_CAPACITY,
  /** The number of available output buffers. */
  IF_TX_CAPACITY,

  /** Unique identifier of the device. */
  IF_DEVICE,
  /** Priority of interrupts or direct memory access requests. */
  IF_PRIORITY,
  /** Data rate. */
  IF_RATE,

  /** Address within the internal address space of the device. */
  IF_ADDRESS,
  /** Alignment of the buffer length for user space buffers. */
  IF_ALIGNMENT,
  /** Size of the internal address space. */
  IF_SIZE,
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
  IF_OPTION_END
};
/*----------------------------------------------------------------------------*/
struct InterfaceClass
{
  CLASS_HEADER

  enum result (*callback)(void *, void (*)(void *), void *);
  enum result (*get)(void *, enum ifOption, void *);
  enum result (*set)(void *, enum ifOption, const void *);
  uint32_t (*read)(void *, uint8_t *, uint32_t);
  uint32_t (*write)(void *, const uint8_t *, uint32_t);
};
/*----------------------------------------------------------------------------*/
struct Interface
{
  struct Entity parent;
};
/*----------------------------------------------------------------------------*/
/**
 * Set callback function and its argument called on completion event.
 * @param interface Pointer to an Interface object.
 * @param callback Callback function.
 * @param argument Callback function argument.
 */
static inline enum result ifCallback(void *interface, void (*callback)(void *),
    void *argument)
{
  return ((const struct InterfaceClass *)CLASS(interface))->callback(interface,
      callback, argument);
}
/*----------------------------------------------------------------------------*/
/**
 * Retrieve the interface option.
 * @param interface Pointer to an Interface object.
 * @param option Option to be read.
 * @param data Pointer to a variable where a value of the option will be stored.
 * @return @b E_OK on success.
 */
static inline enum result ifGet(void *interface, enum ifOption option,
    void *data)
{
  return ((const struct InterfaceClass *)CLASS(interface))->get(interface,
      option, data);
}
/*----------------------------------------------------------------------------*/
/**
 * Set the interface option.
 * @param interface Pointer to an Interface object.
 * @param option Option to be set.
 * @param data Pointer to a new value of the option.
 * @return @b E_OK on success.
 */
static inline enum result ifSet(void *interface, enum ifOption option,
    const void *data)
{
  return ((const struct InterfaceClass *)CLASS(interface))->set(interface,
      option, data);
}
/*----------------------------------------------------------------------------*/
/**
 * Receive data from the interface.
 * @param interface Pointer to an Interface object.
 * @param buffer Pointer to a buffer with a size of at least @b length bytes.
 * @param length Number of bytes to be read.
 * @return The total number of elements successfully read is returned.
 */
static inline uint32_t ifRead(void *interface, uint8_t *buffer, uint32_t length)
{
  return ((const struct InterfaceClass *)CLASS(interface))->read(interface,
      buffer, length);
}
/*----------------------------------------------------------------------------*/
/**
 * Send data over the interface.
 * @param interface Pointer to an Interface object.
 * @param buffer Pointer to a buffer with a size of at least @b length bytes.
 * @param length Number of bytes to be written.
 * @return The total number of elements successfully written is returned.
 */
static inline uint32_t ifWrite(void *interface, const uint8_t *buffer,
    uint32_t length)
{
  return ((const struct InterfaceClass *)CLASS(interface))->write(interface,
      buffer, length);
}
/*----------------------------------------------------------------------------*/
#endif /* INTERFACE_H_ */
