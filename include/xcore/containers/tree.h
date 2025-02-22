/*
 * xcore/containers/tree.h
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CONTAINERS_TREE_H_
#define XCORE_CONTAINERS_TREE_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
struct TreeNode
{
  struct TreeNode *parent;
  struct TreeNode *left;
  struct TreeNode *right;

  /** Difference between right and left sub-tree heights. */
  int balance;

  /** Beginning of the user data. */
  unsigned char data[];
};
/*----------------------------------------------------------------------------*/
struct Tree
{
  /** Comparator function. */
  int (*compare)(const void *, const void *);
  /** Root node. */
  struct TreeNode *root;
  /** Size in bytes of each element. */
  size_t width;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

void treeInit(struct Tree *, size_t, int (*)(const void *, const void *));
void treeDeinit(struct Tree *);
void treeErase(struct Tree *, struct TreeNode *);
struct TreeNode *treeFind(struct Tree *, const void *);
bool treeInsert(struct Tree *, const void *);

void treeClear(struct Tree *);
size_t treeSize(const struct Tree *);

END_DECLS
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline void treeData(const struct Tree *tree,
    const struct TreeNode *node, void *element)
{
  memcpy(element, node->data, tree->width);
}

static inline bool treeEmpty(const struct Tree *tree)
{
  return tree->root == NULL;
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CONTAINERS_TREE_H_ */
