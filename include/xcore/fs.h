/*
 * xcore/fs.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

/**
 * @file
 * Abstract file system interface for embedded applications.
 */

#ifndef XCORE_FS_H_
#define XCORE_FS_H_
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include <xcore/entity.h>
/*----------------------------------------------------------------------------*/
typedef uint8_t FsAccess;
typedef uint32_t FsLength;

enum
{
  /** Read access allows to read data and list directory content. */
  FS_ACCESS_READ = 0x01,
  /** Write access allows to modify data, append or delete nodes. */
  FS_ACCESS_WRITE = 0x02
};

enum FsFieldType
{
  /** Access rights to the node. */
  FS_NODE_ACCESS,
  /** Node payload. */
  FS_NODE_DATA,
  /** Numeric identifier of the node. */
  FS_NODE_ID,
  /** Symbolic name of the node. */
  FS_NODE_NAME,
  /** Owner of the node. */
  FS_NODE_OWNER,
  /** Access time in microseconds. */
  FS_NODE_TIME,

  /** End of the list. */
  FS_TYPE_END
};

struct FsFieldDescriptor
{
  const void *data;
  FsLength length;
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
      FsLength, FsLength *);
  enum Result (*remove)(void *, void *);
  enum Result (*write)(void *, enum FsFieldType, FsLength, const void *,
      FsLength, FsLength *);
};

struct FsNode
{
  struct Entity base;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

/**
 * Get a root node of the file system tree.
 * @param handle Pointer to a file system handle.
 * @return Pointer to a new FsNode object.
 */
static inline void *fsHandleRoot(void *handle)
{
  return ((const struct FsHandleClass *)CLASS(handle))->root(handle);
}

/**
 * Write information about changed entries to the physical device.
 * @param handle Pointer to a file system handle.
 */
static inline enum Result fsHandleSync(void *handle)
{
  return ((const struct FsHandleClass *)CLASS(handle))->sync(handle);
}

/**
 * Create a new node.
 * @param root Root node where the new node should be placed.
 * @param descriptors Pointer to an array of field descriptors
 * with supplementary information about the new node.
 * @param number Number of descriptors in the array.
 * @return E_OK on success.
 */
static inline enum Result fsNodeCreate(void *root,
    const struct FsFieldDescriptor *descriptors, size_t number)
{
  return ((const struct FsNodeClass *)CLASS(root))->create(root,
      descriptors, number);
}

/**
 * Get a first node from the chain of descendant nodes.
 * @param node Pointer to an FsNode object.
 * @return Pointer to a new object with basic information about the first node.
 */
static inline void *fsNodeHead(void *node)
{
  return ((const struct FsNodeClass *)CLASS(node))->head(node);
}

/**
 * Free memory allocated for the node.
 * @param node Pointer to a previously allocated FsNode object.
 */
static inline void fsNodeFree(void *node)
{
  ((const struct FsNodeClass *)CLASS(node))->free(node);
}

/**
 * Read length of the specified field.
 * @param node Pointer to an FsNode object.
 * @param type Unique field identifier.
 * @param length Pointer to a buffer where the result of the operation
 * will be placed.
 * @return @b E_OK on success, @b E_INVALID when the field is not supported.
 */
static inline enum Result fsNodeLength(void *node, enum FsFieldType type,
    FsLength *length)
{
  return ((const struct FsNodeClass *)CLASS(node))->length(node, type, length);
}

/**
 * Read basic information about the next node in the chain.
 * @param node Pointer to an FsNode object.
 * @return @b E_OK on success, @b E_ENTRY when @b node is the last node
 * in the chain.
 */
static inline enum Result fsNodeNext(void *node)
{
  return ((const struct FsNodeClass *)CLASS(node))->next(node);
}

/**
 * Read supplementary information about the node.
 * @param node Pointer to an FsNode object.
 * @param type Unique field identifier.
 * @param position Offset from the beginning of the field.
 * @param buffer Pointer to a buffer with a size of at least @b length bytes.
 * @param length Number of data bytes to be read.
 * @param read Pointer to a buffer where the result of the operation
 * will be placed. May be left zero when the result is not needed.
 * @return @b E_OK on success, @b E_INVALID when this type of operation is not
 * supported.
 */
static inline enum Result fsNodeRead(void *node, enum FsFieldType type,
    FsLength position, void *buffer, FsLength length, FsLength *read)
{
  return ((const struct FsNodeClass *)CLASS(node))->read(node, type, position,
      buffer, length, read);
}

/**
 * Remove entry and make the space it was using available for reuse.
 * @param root Root node where the @b node to be removed is located.
 * @param node Node to be removed.
 * @return @b E_OK on success.
 */
static inline enum Result fsNodeRemove(void *root, void *node)
{
  return ((const struct FsNodeClass *)CLASS(root))->remove(root, node);
}

/**
 * Write supplementary information about the node.
 * @param node Pointer to an FsNode object.
 * @param type Unique field identifier.
 * @param position Offset from the beginning of the data.
 * @param buffer Pointer to a buffer with a size of at least @b length bytes.
 * @param length Number of data bytes to be written.
 * @param written Pointer to a buffer where the result of the operation
 * will be placed. May be left zero when the result is not needed.
 * @return @b E_OK on success, @b E_INVALID when this type of operation is not
 * supported.
 */
static inline enum Result fsNodeWrite(void *node, enum FsFieldType type,
    FsLength position, const void *buffer, FsLength length, FsLength *written)
{
  return ((const struct FsNodeClass *)CLASS(node))->write(node, type, position,
      buffer, length, written);
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_FS_H_ */
