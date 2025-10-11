/*
 * xcore/fs/fs.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

/**
 * @file
 * Abstract file system interface for embedded applications.
 */

#ifndef XCORE_FS_FS_H_
#define XCORE_FS_FS_H_
/*----------------------------------------------------------------------------*/
#include <xcore/entity.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
typedef uint32_t FsAccess;
typedef uint64_t FsCapacity;
typedef uint32_t FsDevice;
typedef uint64_t FsIdentifier;
typedef uint32_t FsLength;
typedef uint32_t FsOwner;

enum
{
  /** Read access allows to read data and list directory content. */
  FS_ACCESS_READ  = 0x01,
  /** Write access allows to modify data, append or delete nodes. */
  FS_ACCESS_WRITE = 0x02
};

enum FsFieldType
{
  /** Access rights to the node. Field type is \a FsAccess. */
  FS_NODE_ACCESS,
  /** Space allocated for the node data. Field type is \a FsCapacity. */
  FS_NODE_CAPACITY,
  /** Node data. */
  FS_NODE_DATA,
  /** Device identifier. Field type is \a FsDevice. */
  FS_NODE_DEVICE,
  /** Numeric identifier of the node. Field type is \a FsIdentifier. */
  FS_NODE_ID,
  /** Symbolic name of the node. Null-terminated array of \a char. */
  FS_NODE_NAME,
  /** Owner of the node. Field type is \a FsOwner. */
  FS_NODE_OWNER,
  /** Access time in microseconds. Field type is \a time64_t. */
  FS_NODE_TIME,

  /** End of the list. */
  FS_TYPE_END
};

struct FsFieldDescriptor
{
  const void *data;
  size_t length;
  enum FsFieldType type;
};
/*----------------------------------------------------------------------------*/
struct FsHandleClass
{
  CLASS_HEADER

  void *(*root)(void *);
  enum Result (*sync)(void *);
};

struct FsHandle
{
  struct Entity base;
};
/*----------------------------------------------------------------------------*/
struct FsNodeClass
{
  CLASS_HEADER

  enum Result (*create)(void *, const struct FsFieldDescriptor *, size_t);
  void *(*head)(void *);
  void (*free)(void *);
  enum Result (*length)(void *, enum FsFieldType, FsLength *);
  enum Result (*next)(void *);
  enum Result (*read)(void *, enum FsFieldType, FsLength, void *,
      size_t, size_t *);
  enum Result (*remove)(void *, void *);
  enum Result (*write)(void *, enum FsFieldType, FsLength, const void *,
      size_t, size_t *);
};

struct FsNode
{
  struct Entity base;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

/**
 * @brief Retrieves the root node of the file system tree.
 * @param[in] handle Pointer to the filesystem handle.
 * @return Pointer to the new FsNode object.
 */
static inline void *fsHandleRoot(void *handle)
{
  return ((const struct FsHandleClass *)CLASS(handle))->root(handle);
}

/**
 * @brief Writes information about modified entries to the physical device.
 * @param[in] handle Pointer to the filesystem handle.
 */
static inline enum Result fsHandleSync(void *handle)
{
  return ((const struct FsHandleClass *)CLASS(handle))->sync(handle);
}

/**
 * @brief Creates a new node in the file system.
 * @param[out] root Root node where the new node will be placed.
 * @param[in] descriptors Pointer to an array of field descriptors
 * containing supplementary information about the new node.
 * @param[in] number Number of descriptors in the array.
 * @return E_OK if successful.
 */
static inline enum Result fsNodeCreate(void *root,
    const struct FsFieldDescriptor *descriptors, size_t number)
{
  return ((const struct FsNodeClass *)CLASS(root))->create(root,
      descriptors, number);
}

/**
 * @brief Retrieves the first node from the chain of descendant nodes.
 * @param[in] node Pointer to the FsNode object.
 * @return Pointer to a new object containing basic information
 * about the first node.
 */
static inline void *fsNodeHead(void *node)
{
  return ((const struct FsNodeClass *)CLASS(node))->head(node);
}

/**
 * @brief Releases memory allocated for the node.
 * @param[in] node Pointer to the previously allocated FsNode object.
 */
static inline void fsNodeFree(void *node)
{
  ((const struct FsNodeClass *)CLASS(node))->free(node);
}

/**
 * @brief Retrieves the length of the specified field.
 * @param[in] node Pointer to the FsNode object.
 * @param[in] type Unique field identifier.
 * @param[out] length Pointer to a buffer where the result will be stored.
 * @return @b E_OK if successful, @b E_INVALID if the field is not supported.
 */
static inline enum Result fsNodeLength(void *node, enum FsFieldType type,
    FsLength *length)
{
  return ((const struct FsNodeClass *)CLASS(node))->length(node, type, length);
}

/**
 * @brief Retrieves basic information about the next node in the chain.
 * @param[in] node Pointer to the FsNode object.
 * @return @b E_OK if successful, @b E_ENTRY if the node is the last
 * in the chain.
 */
static inline enum Result fsNodeNext(void *node)
{
  return ((const struct FsNodeClass *)CLASS(node))->next(node);
}

/**
 * @brief Reads supplementary information about the node.
 *
 * @param[in] node Pointer to the FsNode object.
 * @param[in] type Unique field identifier.
 * @param[in] position Offset from the beginning of the field.
 * @param[out] buffer Pointer to a buffer with a size of
 * at least @b length bytes.
 * @param[in] length Number of data bytes to read.
 * @param[out] read Pointer to a buffer where the result will be stored.
 * May be @b NULL if the result is not needed.
 * @return @b E_OK if successful, @b E_INVALID if the operation
 * is not supported.
 */
static inline enum Result fsNodeRead(void *node, enum FsFieldType type,
    FsLength position, void *buffer, size_t length, size_t *read)
{
  return ((const struct FsNodeClass *)CLASS(node))->read(node, type, position,
      buffer, length, read);
}

/**
 * @brief Removes an entry and frees the space it occupied for reuse.
 * @param[in] root Root node containing the node to be removed.
 * @param[in] node Node to be removed from the file system.
 * @return @b E_OK if the operation was successful.
 */
static inline enum Result fsNodeRemove(void *root, void *node)
{
  return ((const struct FsNodeClass *)CLASS(root))->remove(root, node);
}

/**
 * @brief Writes supplementary information to the node.
 *
 * @param[in] node Pointer to the FsNode object.
 * @param[in] type Unique field identifier for the data.
 * @param[in] position Offset from the beginning of the data field.
 * @param[in] buffer Pointer to the source buffer containing data to write.
 * @param[in] length Number of bytes to write.
 * @param[out] written Pointer to a buffer where the number of written bytes
 * will be stored.
 * Can be @b NULL if this information is not required.
 * @return @b E_OK if the operation was successful,
 * @b E_INVALID if the operation is not supported for this type of node.
 */
static inline enum Result fsNodeWrite(void *node, enum FsFieldType type,
    FsLength position, const void *buffer, size_t length, size_t *written)
{
  return ((const struct FsNodeClass *)CLASS(node))->write(node, type, position,
      buffer, length, written);
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_FS_FS_H_ */
