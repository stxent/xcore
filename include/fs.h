/*
 * fs.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

/**
 * @file
 * Abstract filesystem interface for embedded applications.
 */

#ifndef FS_H_
#define FS_H_
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <entity.h>
#include <error.h>
/*----------------------------------------------------------------------------*/
#ifndef CONFIG_FILENAME_LENGTH
#define CONFIG_FILENAME_LENGTH 32
#endif
/*----------------------------------------------------------------------------*/
typedef uint8_t access_t;
/*----------------------------------------------------------------------------*/
enum
{
  /** Read access to a node. */
  FS_ACCESS_READ = 0x01,
  /** Write access allows to modify and delete nodes. */
  FS_ACCESS_WRITE = 0x02
};
/*----------------------------------------------------------------------------*/
enum fsNodeAttribute
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
  /** Size of the payload in bytes. */
  FS_NODE_SIZE,
  /** Node change time. */
  FS_NODE_TIME
};
/*----------------------------------------------------------------------------*/
struct FsAttributeDescriptor
{
  const void *data;
  uint32_t size;
  enum fsNodeAttribute type;
};
/*----------------------------------------------------------------------------*/
struct FsHandleClass
{
  CLASS_HEADER

  void *(*root)(void *);
  enum result (*sync)(void *);
};
/*----------------------------------------------------------------------------*/
struct FsHandle
{
  struct Entity parent;
};
/*----------------------------------------------------------------------------*/
struct FsNodeClass
{
  CLASS_HEADER

  enum result (*create)(void *, const struct FsAttributeDescriptor *, uint8_t);
  void *(*head)(void *);
  void (*free)(void *);
  enum result (*next)(void *);
  uint32_t (*read)(void *, enum fsNodeAttribute, uint64_t, void *, uint32_t);
  enum result (*remove)(void *, void *);
  uint32_t (*write)(void *, enum fsNodeAttribute, uint64_t, const void *,
      uint32_t);
};
/*----------------------------------------------------------------------------*/
struct FsNode
{
  struct Entity parent;
};
/*----------------------------------------------------------------------------*/
static inline void *fsHandleRoot(void *handle)
{
  return ((const struct FsHandleClass *)CLASS(handle))->root(handle);
}
/*----------------------------------------------------------------------------*/
static inline enum result fsHandleSync(void *handle)
{
  return ((const struct FsHandleClass *)CLASS(handle))->sync(handle);
}
/*----------------------------------------------------------------------------*/
static inline enum result fsNodeCreate(void *parent,
    const struct FsAttributeDescriptor *descriptors, uint8_t count)
{
  return ((const struct FsNodeClass *)CLASS(parent))->create(parent,
      descriptors, count);
}
/*----------------------------------------------------------------------------*/
static inline void *fsNodeHead(void *node)
{
  return ((const struct FsNodeClass *)CLASS(node))->head(node);
}
/*----------------------------------------------------------------------------*/
static inline void fsNodeFree(void *node)
{
  ((const struct FsNodeClass *)CLASS(node))->free(node);
}
/*----------------------------------------------------------------------------*/
static inline enum result fsNodeNext(void *node)
{
  return ((const struct FsNodeClass *)CLASS(node))->next(node);
}
/*----------------------------------------------------------------------------*/
static inline uint32_t fsNodeRead(void *node, enum fsNodeAttribute attribute,
    uint64_t position, void *buffer, uint32_t length)
{
  return ((const struct FsNodeClass *)CLASS(node))->read(node, attribute,
      position, buffer, length);
}
/*----------------------------------------------------------------------------*/
static inline enum result fsNodeRemove(void *parent, void *node)
{
  return ((const struct FsNodeClass *)CLASS(parent))->remove(parent, node);
}
/*----------------------------------------------------------------------------*/
static inline uint32_t fsNodeWrite(void *node, enum fsNodeAttribute attribute,
    uint64_t position, const void *buffer, uint32_t length)
{
  return ((const struct FsNodeClass *)CLASS(node))->write(node, attribute,
      position, buffer, length);
}
/*----------------------------------------------------------------------------*/
#endif /* FS_H_ */
