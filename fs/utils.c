/*
 * utils.c
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <xcore/fs/utils.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
static char *copyPathPart(char *, const char *);
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
bool fsExtractBaseName(char *buffer, const char *path)
{
  assert(buffer);
  assert(path);

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
const char *fsExtractName(const char *path)
{
  assert(path);

  size_t length = 0;

  for (size_t pos = strlen(path); pos > 0; --pos, ++length)
  {
    if (path[pos - 1] == '/')
      return length ? path + pos : 0;
  }

  return length ? path : 0;
}
/*----------------------------------------------------------------------------*/
const char *fsFollowNextPart(struct FsHandle *handle, struct FsNode **node,
    const char *path, bool leaf)
{
  char nextPart[FS_NAME_LENGTH];
  path = fsGetChunk(nextPart, path);

  if (!strlen(nextPart))
  {
    path = 0;
  }
  else if (!strcmp(nextPart, ".") || !strcmp(nextPart, ".."))
  {
    /* Path contains forbidden directories */
    path = 0;
  }
  else if (*node == 0)
  {
    if (nextPart[0] == '/')
    {
      *node = fsHandleRoot(handle);
    }
    else
    {
      /* Relative path is used but the previous state is not available */
      path = 0;
    }
  }
  else if (leaf || strlen(path))
  {
    struct FsNode *child = fsNodeHead(*node);
    fsNodeFree(*node);

    while (child)
    {
      char nodeName[FS_NAME_LENGTH];
      const enum Result res = fsNodeRead(child, FS_NODE_NAME, 0,
          nodeName, sizeof(nodeName), 0);

      if (res == E_OK)
      {
        if (!strcmp(nextPart, nodeName))
          break;
      }

      if (res != E_OK || fsNodeNext(child) != E_OK)
      {
        fsNodeFree(child);
        child = 0;
        break;
      }
    }

    /* Check whether the node is found */
    if (child != 0)
      *node = child;
    else
      path = 0;
  }

  return path;
}
/*----------------------------------------------------------------------------*/
struct FsNode *fsFollowPath(struct FsHandle *handle, const char *path,
    bool leaf)
{
  struct FsNode *node = 0;

  while (path && *path)
    path = fsFollowNextPart(handle, &node, path, leaf);

  return path != 0 ? node : 0;
}
/*----------------------------------------------------------------------------*/
const char *fsGetChunk(char *dst, const char *src)
{
  assert(dst);
  assert(src);

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
void fsJoinPaths(char *buffer, const char *prefix, const char *suffix)
{
  assert(buffer);
  assert(prefix);
  assert(suffix);

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
struct FsNode *fsOpenBaseNode(struct FsHandle *handle, const char *path)
{
  return fsFollowPath(handle, path, false);
}
/*----------------------------------------------------------------------------*/
struct FsNode *fsOpenNode(struct FsHandle *handle, const char *path)
{
  return fsFollowPath(handle, path, true);
}
/*----------------------------------------------------------------------------*/
bool fsStripName(char *buffer)
{
  const char * const position = fsExtractName(buffer);

  if (position)
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
