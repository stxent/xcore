/*
 * fs.h
 * Copyright (C) 2014 xent
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
/*----------------------------------------------------------------------------*/
#ifndef CONFIG_FILENAME_LENGTH
#define CONFIG_FILENAME_LENGTH 64
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
enum fsNodeData
{
  /** Type and symbolic name of the node. */
  FS_NODE_METADATA = 0,
  /** Symbolic name of the node. */
  FS_NODE_NAME,
  /** Type of the node. */
  FS_NODE_TYPE,
  /** Access rights to the node available for user. */
  FS_NODE_ACCESS,
  /** Owner of the node. */
  FS_NODE_OWNER,
  /** Node size in bytes or elements. */
  FS_NODE_SIZE,
  /** Node change time. */
  FS_NODE_TIME
};
/*----------------------------------------------------------------------------*/
enum fsNodeType
{
  /** Unknown type. */
  FS_TYPE_NONE = 0,
  /** Directory entry. */
  FS_TYPE_DIR,
  /** Regular file. */
  FS_TYPE_FILE,
  /** Symbolic link. */
  FS_TYPE_LINK
};
/*----------------------------------------------------------------------------*/
enum fsSeekOrigin
{
  /** Beginning of file. */
  FS_SEEK_SET = 0,
  /** Current position of the file pointer. */
  FS_SEEK_CUR,
  /** End of file. */
  FS_SEEK_END
};
/*----------------------------------------------------------------------------*/
struct FsMetadata
{
  /** Symbolic name of the node. */
  char name[CONFIG_FILENAME_LENGTH];
  /** Type of the node. */
  enum fsNodeType type;
};
/*----------------------------------------------------------------------------*/
struct FsHandleClass
{
  CLASS_HEADER

  void *(*follow)(void *, const char *, const void *);
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

  void *(*clone)(void *);
  void (*free)(void *);
  enum result (*get)(void *, enum fsNodeData, void *);
  enum result (*link)(void *, const struct FsMetadata *, const void *, void *);
  enum result (*make)(void *, const struct FsMetadata *, void *);
  enum result (*mount)(void *, void *);
  void *(*open)(void *, access_t);
  enum result (*set)(void *, enum fsNodeData, const void *);
  enum result (*truncate)(void *);
  enum result (*unlink)(void *);
  void (*unmount)(void *);
};
/*----------------------------------------------------------------------------*/
struct FsNode
{
  struct Entity parent;
};
/*----------------------------------------------------------------------------*/
struct FsEntryClass
{
  CLASS_HEADER

  enum result (*close)(void *);
  bool (*end)(void *);
  enum result (*fetch)(void *, void *);
  uint32_t (*read)(void *, void *, uint32_t);
  enum result (*seek)(void *, uint64_t, enum fsSeekOrigin);
  uint64_t (*tell)(void *);
  uint32_t (*write)(void *, const void *, uint32_t);
};
/*----------------------------------------------------------------------------*/
struct FsEntry
{
  struct Entity parent;
};
/*----------------------------------------------------------------------------*/
/**
 * Follow symbolic path and get node pointing to this entry.
 * @param handle Pointer to an FsHandle object.
 * @param path Path to an entry to be located.
 * @param root Local root entry. Zero value may be used to rewind search and
 * follow the path from global root entry.
 * @return Pointer to an allocated and filled node on success or zero otherwise.
 * Allocated node may belong to other handle when current one
 * has nested partitions.
 */
static inline void *fsFollow(void *handle, const char *path, const void *root)
{
  return ((const struct FsHandleClass *)CLASS(handle))->follow(handle, path,
      root);
}
/*----------------------------------------------------------------------------*/
/**
 * Write information about changed entries to physical device.
 * @param handle Pointer to an FsHandle object.
 */
static inline enum result fsSync(void *handle)
{
  return ((const struct FsHandleClass *)CLASS(handle))->sync(handle);
}
/*----------------------------------------------------------------------------*/
/**
 * Allocate a new node.
 * @param node Pointer to a previously initialized node.
 * @return Pointer to an allocated node on success or zero otherwise.
 */
static inline void *fsClone(void *node)
{
  return ((const struct FsNodeClass *)CLASS(node))->clone(node);
}
/*----------------------------------------------------------------------------*/
/**
 * Free the memory allocated for a node.
 * @param node Pointer to a previously allocated FsNode object.
 */
static inline void fsFree(void *node)
{
  ((const struct FsNodeClass *)CLASS(node))->free(node);
}
/*----------------------------------------------------------------------------*/
/**
 * Load information about the node.
 * @param node The node with information about entry location.
 * @param type Information type.
 * @param data Pointer to a specified buffer to be filled.
 * @return @b E_OK on success.
 */
static inline enum result fsGet(void *node, enum fsNodeData type,
    void *data)
{
  return ((const struct FsNodeClass *)CLASS(node))->get(node, type, data);
}
/*----------------------------------------------------------------------------*/
/**
 * Create a link to an existing entry.
 * @param node The node pointing to a location where the link should be placed.
 * @param metadata Pointer to a link information.
 * @param target The node pointing to an existing entry.
 * @param result Pointer to a previously allocated node where the information
 * about newly created entry should be stored. May be left zero when such
 * information is not needed.
 * @return E_OK on success.
 */
static inline enum result fsLink(void *node, const struct FsMetadata *metadata,
    const void *target, void *result)
{
  return ((const struct FsNodeClass *)CLASS(node))->link(node, metadata, target,
      result);
}
/*----------------------------------------------------------------------------*/
/**
 * Create a new entry.
 * @param node The node pointing to a location where new entry should be placed.
 * @param metadata Pointer to an entry information.
 * @param result Pointer to a previously allocated node where the information
 * about newly created entry should be stored. May be left zero when such
 * information is not needed.
 * @return E_OK on success.
 */
static inline enum result fsMake(void *node, const struct FsMetadata *metadata,
    void *result)
{
  return ((const struct FsNodeClass *)CLASS(node))->make(node, metadata,
      result);
}
/*----------------------------------------------------------------------------*/
/**
 * Link filesystem handle with existing node.
 * @param node The node pointing to an existing directory node.
 * @param handle Pointer to an initialized filesystem handle.
 * @return E_OK on success.
 */
static inline enum result fsMount(void *node, void *handle)
{
  return ((const struct FsNodeClass *)CLASS(node))->mount(node, handle);
}
/*----------------------------------------------------------------------------*/
/**
 * Open an entry.
 * @param node The node with information about entry location.
 * @param access Requested access rights.
 * @return Pointer to a newly allocated entry on success or zero on error.
 */
static inline void *fsOpen(void *node, access_t access)
{
  return ((const struct FsNodeClass *)CLASS(node))->open(node, access);
}
/*----------------------------------------------------------------------------*/
/**
 * Modify information about the node.
 * @param node The node with information about entry location.
 * @param type Information type.
 * @param data Pointer to a specified buffer to be saved.
 * @return @b E_OK on success.
 */
static inline enum result fsSet(void *node, enum fsNodeData type,
    const void *data)
{
  return ((const struct FsNodeClass *)CLASS(node))->set(node, type, data);
}
/*----------------------------------------------------------------------------*/
/**
 * Truncate node data.
 * @param node The node with information about entry location.
 * @return E_OK on success.
 */
static inline enum result fsTruncate(void *node)
{
  return ((const struct FsNodeClass *)CLASS(node))->truncate(node);
}
/*----------------------------------------------------------------------------*/
/**
 * Remove entry information but preserve data.
 * @param node The node with information about entry location.
 * @return @b E_OK on success.
 */
static inline enum result fsUnlink(void *node)
{
  return ((const struct FsNodeClass *)CLASS(node))->unlink(node);
}
/*----------------------------------------------------------------------------*/
/**
 * Break the link between filesystem handles.
 * @param node The node with connection to other handle.
 */
static inline void fsUnmount(void *node)
{
  ((const struct FsNodeClass *)CLASS(node))->unmount(node);
}
/*----------------------------------------------------------------------------*/
/**
 * Close entry and free descriptor data.
 * @param entry Pointer to a previously opened entry.
 */
static inline enum result fsClose(void *entry)
{
  return ((const struct FsEntryClass *)CLASS(entry))->close(entry);
}
/*----------------------------------------------------------------------------*/
/**
 * Check whether a position in entry reached the end of entry.
 * @param entry Pointer to a previously opened entry.
 * @return @b true when reached the end of entry or @b false otherwise.
 */
static inline bool fsEnd(void *entry)
{
  return ((const struct FsEntryClass *)CLASS(entry))->end(entry);
}
/*----------------------------------------------------------------------------*/
/**
 * Read next directory entry.
 * @param entry Pointer to a previously opened entry.
 * @param node Pointer to a previously allocated node where the information
 * about next entry should be stored.
 * @return E_OK on success.
 */
static inline enum result fsFetch(void *entry, void *node)
{
  return ((const struct FsEntryClass *)CLASS(entry))->fetch(entry, node);
}
/*----------------------------------------------------------------------------*/
/**
 * Read block of data from the entry.
 * @param entry Pointer to a previously opened entry.
 * @param buffer Pointer to a buffer with a size of at least @b length bytes.
 * @param length Number of data bytes to be read.
 * @return E_OK on success.
 */
static inline uint32_t fsRead(void *entry, void *buffer, uint32_t length)
{
  return ((const struct FsEntryClass *)CLASS(entry))->read(entry, buffer,
      length);
}
/*----------------------------------------------------------------------------*/
/**
 * Set the position in the entry to a new position.
 * @param entry Pointer to a previously opened entry.
 * @param offset Number of bytes to offset from origin.
 * @param origin Position used as reference for the offset.
 * @return E_OK on success.
 */
static inline enum result fsSeek(void *entry, uint64_t offset,
    enum fsSeekOrigin origin)
{
  return ((const struct FsEntryClass *)CLASS(entry))->seek(entry, offset,
      origin);
}
/*----------------------------------------------------------------------------*/
/**
 * Get the current position in entry.
 * @param entry Pointer to a previously opened entry.
 * @return Position in a file.
 */
static inline uint64_t fsTell(void *entry)
{
  return ((const struct FsEntryClass *)CLASS(entry))->tell(entry);
}
/*----------------------------------------------------------------------------*/
/**
 * Write block of data to entry.
 * @param entry Pointer to a previously opened entry.
 * @param buffer Pointer to a buffer with a size of at least @b length bytes.
 * @param length Number of data bytes to be written.
 * @return E_OK on success.
 */
static inline uint32_t fsWrite(void *entry, const void *buffer, uint32_t length)
{
  return ((const struct FsEntryClass *)CLASS(entry))->write(entry, buffer,
      length);
}
/*----------------------------------------------------------------------------*/
#endif /* FS_H_ */
