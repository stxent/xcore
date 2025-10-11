/*
 * utils.c
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/fs/utils.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
static enum Result computeNodeUsage(struct FsNode *, FsCapacity *);
static char *copyPathPart(char *, const char *);
static bool isReservedName(const char *);
/*----------------------------------------------------------------------------*/
static enum Result computeNodeUsage(struct FsNode *node, FsCapacity *result)
{
  struct FsNode *child = fsNodeHead(node);
  FsCapacity capacity;
  enum Result res;

  res = fsNodeRead(node, FS_NODE_CAPACITY, 0, &capacity, sizeof(capacity),
      NULL);
  if (res == E_OK)
    *result += capacity;
  res = E_OK;

  while (child != NULL)
  {
    FsLength nameLength;
    bool reserved = false;

    if (fsNodeLength(child, FS_NODE_NAME, &nameLength) == E_OK)
    {
      if (nameLength >= 2 && nameLength <= 3)
      {
        char name[FS_NAME_LENGTH];

        res = fsNodeRead(child, FS_NODE_NAME, 0, name, sizeof(name), NULL);
        if (res != E_OK)
          break;

        reserved = isReservedName(name);
      }
    }

    if (!reserved)
    {
      res = computeNodeUsage(child, result);
      if (res != E_OK)
        break;
    }

    res = fsNodeNext(child);
    if (res != E_OK)
    {
      if (res == E_ENTRY)
        res = E_OK;
      break;
    }
  }

  if (child != NULL)
    fsNodeFree(child);

  return res;
}
/*----------------------------------------------------------------------------*/
static char *copyPathPart(char *output, const char *input)
{
  /* Prepend output with slash */
  if (*input != '/')
    *output++ = '/';

  do
  {
    /* Replace continuous slashes with single slash */
    if (*input == '/')
    {
      while (*++input == '/');
      *output++ = '/';
    }
    else
      *output++ = *input++;
  }
  while (*input);

  /* Remove trailing slash if exists */
  if (*(output - 1) == '/')
    --output;

  return output;
}
/*----------------------------------------------------------------------------*/
static bool isReservedName(const char *name)
{
  return name[0] == '.' && (!name[1] || (name[1] == '.' && !name[2]));
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Extracts directories name from a path.
 * @param[out] buffer Buffer to store path without a leaf name
 * or a last directory name.
 * @param[in] path Input path.
 * @return @b true if base name was extracted @b false otherwise.
 */
bool fsExtractBaseName(char *buffer, const char *path)
{
  assert(buffer != NULL);
  assert(path != NULL);

  size_t length = 0;

  for (size_t pos = strlen(path); pos > 0; --pos, ++length)
  {
    if (path[pos - 1] == '/')
    {
      if (pos > 1)
      {
        memcpy(buffer, path, pos - 1);
        buffer[pos - 1] = '\0';

        return true;
      }
      else
        return false;
    }
  }

  return false;
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Extracts the last component from a path.
 * @param[in] path Input path.
 * @return Pointer to the last component or @b NULL.
 */
const char *fsExtractName(const char *path)
{
  assert(path != NULL);

  size_t length = 0;

  for (size_t pos = strlen(path); pos > 0; --pos, ++length)
  {
    if (path[pos - 1] == '/')
      return length ? path + pos : NULL;
  }

  return length ? path : NULL;
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Finds the used space in the file system.
 * @param[in] handle Pointer to file system handle.
 * @param[in] node Starting node or @b NULL for root.
 * @return Used space in bytes.
 */
FsCapacity fsFindUsedSpace(struct FsHandle *handle, struct FsNode *node)
{
  struct FsNode * const parent = node ? node : fsHandleRoot(handle);

  if (parent != NULL)
  {
    FsCapacity used = 0;
    const enum Result res = computeNodeUsage(parent, &used);

    if (node == NULL)
      fsNodeFree(parent);

    return res == E_OK ? used : 0;
  }
  else
    return 0;
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Follows the next path part.
 * @param[in] handle Pointer to file system handle.
 * @param[in,out] node Pointer to current node.
 * @param[in] path Path to follow.
 * @param[in] leaf Whether to treat as leaf node.
 * @return Remaining path or @b NULL on error.
 */
const char *fsFollowNextPart(struct FsHandle *handle, struct FsNode **node,
    const char *path, bool leaf)
{
  char nextPart[FS_NAME_LENGTH];
  path = fsGetChunk(nextPart, path);

  if (!strlen(nextPart))
  {
    path = NULL;
  }
  else if (isReservedName(nextPart))
  {
    /* Path contains forbidden directories */
    path = NULL;
  }
  else if (*node == NULL)
  {
    if (nextPart[0] == '/')
    {
      *node = fsHandleRoot(handle);
    }
    else
    {
      /* Relative path is used but the previous state is not available */
      path = NULL;
    }
  }
  else if (leaf || strlen(path))
  {
    struct FsNode *child = fsNodeHead(*node);
    fsNodeFree(*node);

    while (child != NULL)
    {
      char nodeName[FS_NAME_LENGTH];
      enum Result res;

      res = fsNodeRead(child, FS_NODE_NAME, 0, nodeName, sizeof(nodeName),
          NULL);

      if (res == E_OK && !strcmp(nextPart, nodeName))
      {
        break;
      }

      res = fsNodeNext(child);

      if (res != E_OK)
      {
        fsNodeFree(child);
        child = NULL;
        break;
      }
    }

    /* Check whether the node is found */
    if (child != NULL)
      *node = child;
    else
      path = NULL;
  }

  return path;
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Follows a full path in the file system.
 * @param[in] handle Pointer to file system handle.
 * @param[in] path Path to follow.
 * @param[in] leaf Whether to treat as leaf node.
 * @return Pointer to the resulting node or @b NULL.
 */
struct FsNode *fsFollowPath(struct FsHandle *handle, const char *path,
    bool leaf)
{
  struct FsNode *node = NULL;

  while (path != NULL && *path)
    path = fsFollowNextPart(handle, &node, path, leaf);

  return path != NULL ? node : NULL;
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Extracts a path chunk into a buffer.
 * @param[out] dst Destination buffer.
 * @param[in] src Source path.
 * @return Remaining path after the chunk.
 */
const char *fsGetChunk(char *dst, const char *src)
{
  assert(dst != NULL);
  assert(src != NULL);

  /* Output buffer length should be greater or equal to maximum name length */
  size_t counter = 0;

  if (!*src)
  {
    *dst = '\0';
    return src;
  }

  if (*src == '/')
  {
    /* Strip leading slashes */
    while (*++src == '/');

    *dst++ = '/';
    *dst = '\0';
    return src;
  }

  while (*src && counter++ < FS_NAME_LENGTH - 1)
  {
    if (*src == '/')
    {
      /* Strip trailing slashes */
      while (*++src == '/');

      break;
    }
    *dst++ = *src++;
  }
  *dst = '\0';

  return src;
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Joins two paths together.
 * @param[out] buffer Resulting path buffer.
 * @param[in] prefix Prefix path.
 * @param[in] suffix Suffix path.
 */
void fsJoinPaths(char *buffer, const char *prefix, const char *suffix)
{
  assert(buffer != NULL);
  assert(prefix != NULL);
  assert(suffix != NULL);

  const char * const begin = buffer;

  if (*suffix != '/' && *prefix != '\0')
  {
    buffer = copyPathPart(buffer, prefix);
  }

  if (*suffix != '\0')
  {
    buffer = copyPathPart(buffer, suffix);
  }

  if (buffer == begin)
    *buffer++ = '/';

  *buffer = '\0';
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Opens a base node by path.
 * @param[in] handle Pointer to file system handle.
 * @param[in] path Path to the node.
 * @return Pointer to the opened node or @b NULL.
 */
struct FsNode *fsOpenBaseNode(struct FsHandle *handle, const char *path)
{
  return fsFollowPath(handle, path, false);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Opens a node by path.
 * @param[in] handle Pointer to file system handle.
 * @param[in] path Path to the node.
 * @return Pointer to the opened node or @b NULL.
 */
struct FsNode *fsOpenNode(struct FsHandle *handle, const char *path)
{
  return fsFollowPath(handle, path, true);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Removes a leap name or directory name from path.
 * @param[in,out] buffer Path buffer.
 * @param[in] path Path to the node.
 * @return @b true when name was removed or @b false otherwise.
 */
bool fsStripName(char *buffer)
{
  const char * const position = fsExtractName(buffer);

  if (position != NULL)
  {
    size_t offset = position - buffer;

    if (offset > 1)
      --offset;
    buffer[offset] = '\0';

    return true;
  }
  else
    return false;
}
