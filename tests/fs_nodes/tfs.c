/*
 * tfs.c
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

#include "tfs.h"
#include <xcore/containers/tg_list.h>
#include <assert.h>
/*----------------------------------------------------------------------------*/
#define INITIAL_LENGTH 16

DEFINE_LIST(void *, Pointer, pointer)
/*----------------------------------------------------------------------------*/
struct TfsNode;

struct TfsHandle
{
  struct FsHandle base;
  struct TfsNode *root;
};

struct TfsNodeConfig
{
  struct TfsHandle *handle;
  struct TfsNode *parent;
  const char *name;
};

struct TfsNode
{
  struct Entity base;

  struct TfsHandle *handle;
  struct TfsNode *parent;
  PointerList children;
  char *name;

  size_t dataCapacity;
  size_t dataLength;
  uint8_t *dataBuffer;
  FsAccess access;
};

struct TfsNodeProxyConfig
{
  struct TfsNode *node;
};

struct TfsNodeProxy
{
  struct FsNode base;

  struct TfsNode *node;
};
/*----------------------------------------------------------------------------*/
static bool reallocateDataBuffer(struct TfsNode *, size_t);
static bool renameNode(struct TfsNode *, const char *);
static enum Result writeDataBuffer(struct TfsNode *, FsLength,
    const void *, size_t, size_t *);
/*----------------------------------------------------------------------------*/
/* Handle functions */
static enum Result tfsHandleInit(void *, const void *);
static void tfsHandleDeinit(void *);
static void *tfsHandleRoot(void *);
static enum Result tfsHandleSync(void *);

/* Node functions */
static enum Result tfsNodeInit(void *, const void *);
static void tfsNodeDeinit(void *);

/* Node proxy functions */
static enum Result tfsNodeProxyInit(void *, const void *);
static void tfsNodeProxyDeinit(void *);
static enum Result tfsNodeProxyCreate(void *, const struct FsFieldDescriptor *,
    size_t);
static void *tfsNodeProxyHead(void *);
static void tfsNodeProxyFree(void *);
static enum Result tfsNodeProxyLength(void *, enum FsFieldType, FsLength *);
static enum Result tfsNodeProxyNext(void *);
static enum Result tfsNodeProxyRead(void *, enum FsFieldType, FsLength,
    void *, size_t, size_t *);
static enum Result tfsNodeProxyRemove(void *, void *);
static enum Result tfsNodeProxyWrite(void *, enum FsFieldType, FsLength,
    const void *, size_t, size_t *);
/*----------------------------------------------------------------------------*/
const struct FsHandleClass * const TfsHandle = &(const struct FsHandleClass){
    .size = sizeof(struct TfsHandle),
    .init = tfsHandleInit,
    .deinit = tfsHandleDeinit,

    .root = tfsHandleRoot,
    .sync = tfsHandleSync
};

const struct EntityClass * const TfsNode = &(const struct EntityClass){
    .size = sizeof(struct TfsNode),
    .init = tfsNodeInit,
    .deinit = tfsNodeDeinit
};

const struct FsNodeClass * const TfsNodeProxy = &(const struct FsNodeClass){
    .size = sizeof(struct TfsNodeProxy),
    .init = tfsNodeProxyInit,
    .deinit = tfsNodeProxyDeinit,

    .create = tfsNodeProxyCreate,
    .head = tfsNodeProxyHead,
    .free = tfsNodeProxyFree,
    .length = tfsNodeProxyLength,
    .next = tfsNodeProxyNext,
    .read = tfsNodeProxyRead,
    .remove = tfsNodeProxyRemove,
    .write = tfsNodeProxyWrite
};
/*----------------------------------------------------------------------------*/
static bool reallocateDataBuffer(struct TfsNode *node, size_t length)
{
  assert(length > node->dataCapacity);

  size_t dataCapacity = node->dataCapacity;

  if (!dataCapacity)
    dataCapacity = INITIAL_LENGTH;
  while (dataCapacity < length)
    dataCapacity *= 2;

  uint8_t * const reallocatedDataBuffer = malloc(dataCapacity);

  if (reallocatedDataBuffer == NULL)
    return false;

  if (node->dataLength > 0)
    memcpy(reallocatedDataBuffer, node->dataBuffer, node->dataLength);
  node->dataBuffer = reallocatedDataBuffer;
  node->dataCapacity = dataCapacity;

  return true;
}
/*----------------------------------------------------------------------------*/
static bool renameNode(struct TfsNode *node, const char *name)
{
  const size_t length = name ? strlen(name) : 0;

  if (length > 0)
  {
    char * const buffer = malloc(length + 1);

    if (buffer != NULL)
    {
      strcpy(buffer, name);

      free(node->name);
      node->name = buffer;
      return true;
    }
    else
      return false;
  }
  else
  {
    free(node->name);
    node->name = NULL;
    return true;
  }
}
/*----------------------------------------------------------------------------*/
static enum Result writeDataBuffer(struct TfsNode *node, FsLength position,
    const void *buffer, size_t length, size_t *written)
{
  const size_t end = position + length;

  if (end > node->dataCapacity)
  {
    if (!reallocateDataBuffer(node, position + length))
      return E_MEMORY;
  }

  memcpy(node->dataBuffer + position, buffer, length);
  if (end > node->dataLength)
    node->dataLength = end;

  if (written != NULL)
    *written = length;

  return E_OK;
}
/*----------------------------------------------------------------------------*/
static enum Result tfsHandleInit(void *object,
    const void *configBase __attribute__((unused)))
{
  struct TfsHandle * const handle = object;

  const struct TfsNodeConfig config = {
      .handle = handle,
      .parent = NULL,
      .name = NULL
  };
  handle->root = init(TfsNode, &config);
  return handle->root ? E_OK : E_MEMORY;
}
/*----------------------------------------------------------------------------*/
static void tfsHandleDeinit(void *object)
{
  struct TfsHandle * const handle = object;

  /* TODO Recursive deinit */
  deinit(handle->root);
}
/*----------------------------------------------------------------------------*/
static void *tfsHandleRoot(void *object)
{
  struct TfsHandle * const handle = object;

  const struct TfsNodeProxyConfig config = {
      .node = handle->root
  };
  return init(TfsNodeProxy, &config);
}
/*----------------------------------------------------------------------------*/
static enum Result tfsHandleSync(void *object __attribute__((unused)))
{
  return E_OK;
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeInit(void *object, const void *configBase)
{
  const struct TfsNodeConfig * const config = configBase;
  struct TfsNode * const node = object;

  node->handle = config->handle;
  node->parent = config->parent;
  pointerListInit(&node->children);

  if (config->name != NULL)
  {
    node->name = malloc(strlen(config->name) + 1);

    if (node->name == NULL)
      return E_MEMORY;

    strcpy(node->name, config->name);
  }
  else
    node->name = NULL;

  node->dataCapacity = 0;
  node->dataLength = 0;
  node->dataBuffer = NULL;
  node->access = FS_ACCESS_READ | FS_ACCESS_WRITE;

  return E_OK;
}
/*----------------------------------------------------------------------------*/
static void tfsNodeDeinit(void *object)
{
  struct TfsNode * const node = object;
  free(node->name);
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeProxyInit(void *object, const void *configBase)
{
  const struct TfsNodeProxyConfig * const config = configBase;
  struct TfsNodeProxy * const proxy = object;

  proxy->node = config->node;
  return E_OK;
}
/*----------------------------------------------------------------------------*/
static void tfsNodeProxyDeinit(void *object __attribute__((unused)))
{
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeProxyCreate(void *object,
    const struct FsFieldDescriptor *descriptors, size_t number)
{
  const struct TfsNodeProxy * const proxy = object;
  struct TfsNode * const node = proxy->node;
  const struct FsFieldDescriptor *name = NULL;

  if (!(node->access & FS_ACCESS_WRITE))
    return E_ACCESS;

  for (size_t index = 0; index < number; ++index)
  {
    const struct FsFieldDescriptor * const desc = descriptors + index;

    switch (desc->type)
    {
      case FS_NODE_NAME:
        if (desc->length == strlen(desc->data) + 1)
        {
          name = desc;
          break;
        }
        else
          return E_VALUE;

      default:
        break;
    }
  }

  if (name != NULL)
  {
    const struct TfsNodeConfig config = {
        .handle = node->handle,
        .parent = node,
        .name = name->data
    };
    struct TfsNode * const child = init(TfsNode, &config);

    if (child == NULL)
      return E_MEMORY;

    if (!pointerListPushFront(&node->children, child))
    {
      deinit(child);
      return E_MEMORY;
    }
    else
      return E_OK;
  }
  else
    return E_VALUE;
}
/*----------------------------------------------------------------------------*/
static void *tfsNodeProxyHead(void *object)
{
  const struct TfsNodeProxy * const proxy = object;
  struct TfsNode * const node = proxy->node;

  if (!(node->access & FS_ACCESS_READ))
    return 0;

  PointerListNode *current = pointerListFront(&node->children);

  if (current != NULL)
  {
    const struct TfsNodeProxyConfig config = {
        .node = *pointerListData(current)
    };
    return init(TfsNodeProxy, &config);
  }
  else
    return 0;
}
/*----------------------------------------------------------------------------*/
static void tfsNodeProxyFree(void *object)
{
  deinit(object);
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeProxyLength(void *object, enum FsFieldType type,
    FsLength *length)
{
  const struct TfsNodeProxy * const proxy = object;
  const struct TfsNode * const node = proxy->node;

  switch (type)
  {
    case FS_NODE_ACCESS:
      if (length != NULL)
        *length = (FsLength)sizeof(FsAccess);
      return E_OK;

    case FS_NODE_CAPACITY:
      if (length != NULL)
        *length = (FsLength)sizeof(FsCapacity);
      return E_OK;

    case FS_NODE_DATA:
      if (length != NULL)
        *length = (FsLength)node->dataLength;
      return E_OK;

    case FS_NODE_NAME:
      if (node->name == NULL)
        return E_INVALID;

      if (length != NULL)
        *length = (FsLength)(strlen(node->name) + 1);
      return E_OK;

    default:
      return E_INVALID;
  }
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeProxyNext(void *object)
{
  struct TfsNodeProxy * const proxy = object;
  struct TfsNode * const parent = proxy->node->parent;

  if (parent == NULL)
    return E_ENTRY;
  if (!(parent->access & FS_ACCESS_READ))
    return E_ACCESS;

  PointerListNode *current = pointerListFront(&parent->children);

  while (current != NULL)
  {
    struct TfsNode * const child = *pointerListData(current);

    if (child == proxy->node)
    {
      current = pointerListNext(current);

      if (current != NULL)
      {
        proxy->node = *pointerListData(current);
        return E_OK;
      }
    }
    else
      current = pointerListNext(current);
  }

  return E_ENTRY;
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeProxyRead(void *object, enum FsFieldType type,
    FsLength position, void *buffer, size_t length, size_t *read)
{
  const struct TfsNodeProxy * const proxy = object;
  const struct TfsNode * const node = proxy->node;

  size_t count = 0;

  switch (type)
  {
    case FS_NODE_ACCESS:
      if (position == 0 && length >= sizeof(node->access))
      {
        memcpy(buffer, &node->access, sizeof(node->access));
        if (read != NULL)
          *read = sizeof(node->access);
        return E_OK;
      }
      else
        return E_VALUE;

    case FS_NODE_CAPACITY:
      if (position == 0 && length >= sizeof(FsCapacity))
      {
        const FsCapacity capacity = (FsCapacity)node->dataCapacity;

        memcpy(buffer, &capacity, sizeof(capacity));
        if (read != NULL)
          *read = sizeof(capacity);
        return E_OK;
      }
      else
        return E_VALUE;

    case FS_NODE_DATA:
    {
      if (!(node->access & FS_ACCESS_READ))
        return E_ACCESS;
      if (position > node->dataLength)
        return E_VALUE;

      const size_t remaining = node->dataLength - position;
      const size_t chunk = MIN(remaining, length);

      memcpy(buffer, node->dataBuffer + position, chunk);
      if (read != NULL)
        *read = chunk;
      return E_OK;
    }

    case FS_NODE_NAME:
    {
      if (node->name == NULL)
        return E_INVALID;

      const size_t nameLength = strlen(node->name) + 1;

      if (position == 0 && length >= nameLength)
      {
        /* Read error simulation */
        if (node->name[0] == '\x7F')
          return E_ERROR;

        strcpy(buffer, node->name);
        count = nameLength;
        break;
      }
      else
        return E_VALUE;
    }

    default:
      return E_INVALID;
  }

  if (read != NULL)
    *read = (FsLength)count;
  return E_OK;
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeProxyRemove(void *rootObject, void *nodeObject)
{
  struct TfsNode * const root = ((struct TfsNodeProxy *)rootObject)->node;
  struct TfsNode * const node = ((struct TfsNodeProxy *)nodeObject)->node;

  if (!(root->access & FS_ACCESS_WRITE) || !(node->access & FS_ACCESS_WRITE))
    return E_ACCESS;
  if (pointerListFind(&root->children, node) == NULL)
    return E_ENTRY;

  pointerListErase(&root->children, node);
  deinit(node);
  return E_OK;
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeProxyWrite(void *object, enum FsFieldType type,
    FsLength position, const void *buffer, size_t length, size_t *written)
{
  struct TfsNodeProxy * const proxy = object;
  struct TfsNode * const node = proxy->node;

  switch (type)
  {
    case FS_NODE_ACCESS:
      if (position || length != sizeof(node->access))
        return E_VALUE;

      memcpy(&node->access, buffer, sizeof(node->access));
      if (written != NULL)
        *written = sizeof(node->access);
      return E_OK;

    case FS_NODE_DATA:
      if (!(node->access & FS_ACCESS_WRITE))
        return E_ACCESS;

      return writeDataBuffer(node, position, buffer, length, written);

    case FS_NODE_NAME:
      if (position || (length && strnlen(buffer, length) + 1 != length))
        return E_VALUE;

      if (renameNode(node, length ? buffer : 0))
      {
        if (written != NULL)
          *written = length;
        return E_OK;
      }
      else
        return E_MEMORY;

    default:
      return E_INVALID;
  }
}
