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
/** Interface parameters. */
enum IfParameter
{
  /**
   * Number of elements available in the receive queue. Depending on
   * the interface type these may be messages, frames, bytes or other elements.
   * Read-only parameter. Parameter type is \a size_t.
   */
  IF_RX_AVAILABLE,
  /**
   * Free space in the receive queue. Read-only parameter.
   * Parameter type is \a size_t.
   */
  IF_RX_PENDING,
  /**
   * Highest number of elements in the receive queue. Read-only parameter.
   * Parameter type is \a size_t.
   */
  IF_RX_WATERMARK,
  /**
   * Free space in the transmit queue. Read-only parameter.
   * Parameter type is \a size_t.
   */
  IF_TX_AVAILABLE,
  /**
   * Number of pending elements in the transmit queue. Read-only parameter.
   * Parameter type is \a size_t.
   */
  IF_TX_PENDING,
  /**
   * Maximum number of elements in the transmit queue. Read-only parameter.
   * Parameter type is \a size_t.
   */
  IF_TX_WATERMARK,

  /**
   * Unique device identifier, 32-bit value.
   * Parameter type is \a uint32_t.
   */
  IF_ADDRESS,
  /**
   * Unique device identifier, 64-bit value.
   * Parameter type is \a uint64_t.
   */
  IF_ADDRESS_64,
  /** Data transfer rate. Parameter type is \a uint32_t. */
  IF_RATE,

  /** Position in the 32-bit address space. Parameter type is \a uint32_t. */
  IF_POSITION,
  /** Position in the 64-bit address space. Parameter type is \a uint64_t. */
  IF_POSITION_64,
  /**
   * Volume size, 32-bit value. Read-only parameter.
   * Parameter type is \a uint32_t.
   */
  IF_SIZE,
  /**
   * Volume size, 64-bit value. Read-only parameter.
   * Parameter type is \a uint64_t.
   */
  IF_SIZE_64,
  /**
   * Data word size in bits. Read-only parameter.
   * Parameter type is \a size_t.
   */
  IF_WIDTH,

  /**
   * Stop the interface from reading new data. Write-only parameter.
   * A null pointer should be used as a data pointer.
   */
  IF_DISABLE,
  /**
   * Enable reception of new data. Write-only parameter.
   * A null pointer should be used as a data pointer.
   */
  IF_ENABLE,
  /**
   * Flush all cached data. Write-only parameter.
   * A null pointer should be used as a data pointer.
   */
  IF_FLUSH,
  /**
   * Get the execution status of the last command. Returns @b E_OK
   * when the interface is ready to accept a new command. Read-only parameter.
   * A null pointer should be used as a data pointer.
   */
  IF_STATUS,

  /**
   * Select blocking mode for the interface. Read and write operations are
   * blocked until all data has been received or transmitted.
   * Write-only parameter. A null pointer should be used as a data pointer.
   */
  IF_BLOCKING,
  /**
   * Select zero-copy mode for the interface. Read and write operations
   * will not block and will immediately return the number of items added.
   * User space buffers are unavailable until the transfer is complete.
   * Write-only parameter. A null pointer should be used as a data pointer.
   */
  IF_ZEROCOPY,

  /**
   * Acquire the interface. Returns @b E_OK on success or @b E_BUSY.
   * Write-only parameter. A null pointer should be used as a data pointer.
   */
  IF_ACQUIRE,
  /**
   * Release the interface. Write-only parameter. When supported
   * always returns @b E_OK. A null pointer should be used as a data pointer.
   */
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
 * @brief Sets the callback function and its argument to be called
 * on completion event.
 * @param[in] interface Pointer to an Interface object.
 * @param[in] callback Callback function to be invoked.
 * @param[in] argument Argument passed to the callback function.
 */
static inline void ifSetCallback(void *interface, void (*callback)(void *),
    void *argument)
{
  ((const struct InterfaceClass *)CLASS(interface))->setCallback(interface,
      callback, argument);
}

/**
 * @brief Reads the specified interface parameter.
 * @param[in] interface Pointer to an Interface object.
 * @param[in] parameter Parameter identifier to be read.
 * @param[out] data Pointer to a variable where the parameter value
 * will be stored.
 * @return @b E_OK if the operation was successful.
 */
static inline enum Result ifGetParam(void *interface, int parameter, void *data)
{
  return ((const struct InterfaceClass *)CLASS(interface))->getParam(interface,
      parameter, data);
}

/**
 * @brief Sets a new value for the interface parameter.
 * @param[in] interface Pointer to an Interface object.
 * @param[in] parameter Parameter identifier to be set.
 * @param[in] data Pointer to the new parameter value.
 * @return @b E_OK if the operation was successful.
 */
static inline enum Result ifSetParam(void *interface, int parameter,
    const void *data)
{
  return ((const struct InterfaceClass *)CLASS(interface))->setParam(interface,
      parameter, data);
}

/**
 * @brief Receives data from the interface.
 * @param[in] interface Pointer to an Interface object.
 * @param[out] buffer Pointer to a buffer of at least @b length bytes.
 * @param[in] length Number of bytes to be read.
 * @return Number of bytes successfully read.
 */
static inline size_t ifRead(void *interface, void *buffer, size_t length)
{
  return ((const struct InterfaceClass *)CLASS(interface))->read(interface,
      buffer, length);
}

/**
 * @brief Sends data over the interface.
 * @param[in] interface Pointer to an Interface object.
 * @param[in] buffer Pointer to a buffer containing data to write.
 * @param[in] length Number of bytes to write.
 * @return Number of bytes successfully written.
 */
static inline size_t ifWrite(void *interface, const void *buffer, size_t length)
{
  return ((const struct InterfaceClass *)CLASS(interface))->write(interface,
      buffer, length);
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_INTERFACE_H_ */
