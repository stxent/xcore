/*
 * tfs.c
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include "tfs.h"
#include <xcore/containers/tg_list.h>
#include <assert.h>
/*----------------------------------------------------------------------------*/
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
static enum Result tfsHandleInit(void *object,
    const void *configBase __attribute__((unused)))
{
  struct TfsHandle * const handle = object;

  const struct TfsNodeConfig config = {
      .handle = handle,
      .parent = 0,
      .name = 0
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

  if (config->name != 0)
  {
    node->name = malloc(strlen(config->name) + 1);

    if (!node->name)
      return E_MEMORY;

    strcpy(node->name, config->name);
  }
  else
    node->name = 0;

  return E_OK;
}
/*----------------------------------------------------------------------------*/
static void tfsNodeDeinit(void *object)
{
  struct TfsNode * const node = object;

  if (node->name != 0)
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
  const struct FsFieldDescriptor *name = 0;

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

  if (name)
  {
    const struct TfsNodeConfig config = {
        .handle = proxy->node->handle,
        .parent = proxy->node,
        .name = name->data
    };
    struct TfsNode * const node = init(TfsNode, &config);

    if (!node)
      return E_MEMORY;

    if (!pointerListPushFront(&proxy->node->children, node))
    {
      deinit(node);
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
  PointerListNode *current = pointerListFront(&node->children);

  if (current)
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
  /* TODO */
  (void)object;
  (void)type;
  (void)length;
  return E_INVALID;
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeProxyNext(void *object)
{
  struct TfsNodeProxy * const proxy = object;
  struct TfsNode * const parent = proxy->node->parent;

  if (!parent)
    return E_ENTRY;

  PointerListNode *current = pointerListFront(&parent->children);

  while (current)
  {
    struct TfsNode * const child = *pointerListData(current);

    if (child == proxy->node)
    {
      current = pointerListNext(current);

      if (current)
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
    case FS_NODE_NAME:
    {
      const size_t nameLength = strlen(node->name) + 1;

      if (position)
        return E_INVALID;
      if (length < nameLength)
        return E_VALUE;

      strcpy(buffer, node->name);
      count = nameLength;
      break;
    }

    default:
      return E_INVALID;
  }

  if (read)
    *read = (FsLength)count;
  return E_OK;
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeProxyRemove(void *rootObject, void *object)
{
  struct TfsNodeProxy * const rootProxy = rootObject;
  struct TfsNodeProxy * const nodeProxy = object;

  if (pointerListFind(&rootProxy->node->children, nodeProxy->node))
  {
    pointerListErase(&rootProxy->node->children, nodeProxy->node);
    deinit(nodeProxy->node);
    return E_OK;
  }
  else
    return E_ENTRY;
}
/*----------------------------------------------------------------------------*/
static enum Result tfsNodeProxyWrite(void *object, enum FsFieldType type,
    FsLength position, const void *buffer, size_t length, size_t *written)
{
  /* TODO */
  (void)object;
  (void)type;
  (void)position;
  (void)buffer;
  (void)length;
  (void)written;

  return E_INVALID;
}
