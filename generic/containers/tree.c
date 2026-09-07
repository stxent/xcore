/*
 * tree.c
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/tree.h>
#include <stdint.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
static inline size_t getNodeSize(size_t);
static void makeNodeList(struct Tree *, size_t, size_t);
static inline struct TreeNode *nodeAllocate(struct Tree *);
static inline void nodeRelease(struct Tree *, struct TreeNode *);

static struct TreeNode *fetchFirst(struct TreeNode *);
static struct TreeNode *fetchNextInOrder(struct TreeNode *);
static struct TreeNode *fetchNextPostOrder(struct TreeNode *);
static void insertNode(struct Tree *, struct TreeNode *);
static struct TreeNode *rotateLeft(struct TreeNode *);
static struct TreeNode *rotateRight(struct TreeNode *);

static struct TreeNode *rotateLeftLeft(struct TreeNode *);
static struct TreeNode *rotateLeftRight(struct TreeNode *);
static struct TreeNode *rotateRightRight(struct TreeNode *);
static struct TreeNode *rotateRightLeft(struct TreeNode *);
/*----------------------------------------------------------------------------*/
static inline size_t getNodeSize(size_t width)
{
  return offsetof(struct TreeNode, data)
      + ((width + (sizeof(uintptr_t) - 1)) & ~(sizeof(uintptr_t) - 1));
}
/*----------------------------------------------------------------------------*/
static void makeNodeList(struct Tree *tree, size_t size, size_t capacity)
{
  struct TreeNode *current = (struct TreeNode *)tree->data;

  for (size_t index = 0; index < capacity - 1; ++index)
  {
    current->left = (struct TreeNode *)((uintptr_t)current + size);
    current = current->left;
  }
  current->left = nullptr;
}
/*----------------------------------------------------------------------------*/
static inline struct TreeNode *nodeAllocate(struct Tree *tree)
{
  if (tree->pool != nullptr)
  {
    struct TreeNode * const node = tree->pool;

    tree->pool = tree->pool->left;
    return node;
  }
  else
    return nullptr;
}
/*----------------------------------------------------------------------------*/
static inline void nodeRelease(struct Tree *tree, struct TreeNode *node)
{
  node->left = tree->pool;
  tree->pool = node;
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *fetchFirst(struct TreeNode *node)
{
  struct TreeNode *current = node;

  while (current->left != nullptr)
    current = current->left;

  return current;
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *fetchNextInOrder(struct TreeNode *node)
{
  if (node->right != nullptr)
  {
    struct TreeNode *current = node->right;

    while (current != nullptr && current->left != nullptr)
      current = current->left;

    return current;
  }

  struct TreeNode *parent = node->parent;

  while (parent != nullptr && parent->right == node)
  {
    node = parent;
    parent = parent->parent;
  }

  return parent;
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *fetchNextPostOrder(struct TreeNode *node)
{
  struct TreeNode * const parent = node->parent;

  if (parent == nullptr)
    return nullptr;

  if (parent->right == node || parent->right == nullptr)
    return parent;

  struct TreeNode *current = parent->right;

  while (current != nullptr
      && (current->left != nullptr || current->right != nullptr))
  {
    current = current->left != nullptr ? current->left : current->right;
  }

  return current;
}
/*----------------------------------------------------------------------------*/
static void insertNode(struct Tree *tree, struct TreeNode *node)
{
  struct TreeNode *current = tree->root;
  struct TreeNode *parent = nullptr;
  bool left = false;

  while (current != nullptr)
  {
    parent = current;

    left = tree->compare(node->data, current->data) < 0;
    current = left ? current->left : current->right;
  }

  if (parent != nullptr)
  {
    node->parent = parent;
    current = node;

    if (left)
      parent->left = node;
    else
      parent->right = node;
  }
  else
  {
    tree->root = node;
    return;
  }

  while (parent != nullptr)
  {
    const int newBalance = parent->balance + (parent->left == current ? -1 : 1);
    struct TreeNode * const ancestor = parent->parent;
    struct TreeNode *root = nullptr;
    bool propagate = false;

    if (newBalance <= -2)
    {
      /* Left heavy */
      if (parent->left->balance >= 1)
        root = rotateLeftRight(parent);
      else
        root = rotateRightRight(parent);
    }
    else if (newBalance >= 2)
    {
      /* Right heavy */
      if (parent->right->balance <= -1)
        root = rotateRightLeft(parent);
      else
        root = rotateLeftLeft(parent);
    }
    else
    {
      if (newBalance)
        propagate = true;
      parent->balance = newBalance;
    }

    if (root != nullptr)
    {
      if (ancestor != nullptr)
      {
        if (ancestor->left == parent)
          ancestor->left = root;
        else
          ancestor->right = root;
        parent = root;
      }
      else
      {
        tree->root = root;
        break;
      }
    }

    if (!propagate)
      break;

    current = parent;
    parent = ancestor;
  }
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *rotateLeft(struct TreeNode *node)
{
  struct TreeNode * const a = node;
  struct TreeNode * const b = a->right;

  a->right = b->left;
  b->left = a;

  b->parent = a->parent;
  a->parent = b;
  b->left->parent = b;
  if (a->right != nullptr)
    a->right->parent = a;

  return b;
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *rotateRight(struct TreeNode *node)
{
  struct TreeNode * const a = node;
  struct TreeNode * const b = a->left;

  a->left = b->right;
  b->right = a;

  b->parent = a->parent;
  a->parent = b;
  b->right->parent = b;
  if (a->left != nullptr)
    a->left->parent = a;

  return b;
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *rotateLeftLeft(struct TreeNode *node)
{
  const int balance = node->right->balance;

  /*
   *  a
   * e  b
   *   c d
   *
   *    b
   *  a  d
   * e c
   *
   * BFa = 2
   * BFb = 0 or 1
   * BFa = Har - Hal = Har - (1 + max(Hbr, Hbl))
   * BFb = Hbr - Hbl
   * Hbl = Hbr - BFb
   * BFb == 1:
   *   Hal + 2 = Hbr + 1
   *   Hbr - Hal = 1
   *   BFa = Har - (Hbr - 1) = Har - Hbr + 1
   *   Har - Hbr = BFa - 1
   *
   * BFb' = Hbr - (1 + max(Har, Hbl)) = Hbr - 1 - Hbl = BFb - 1
   * BFa' = Hbl - Hal = Hbr - BFb - Hal = Hbr - Hal - BFb = 1 - BFb
   */

  node = rotateLeft(node);

  node->balance = balance - 1;
  node->left->balance = 1 - balance;

  return node;
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *rotateLeftRight(struct TreeNode *node)
{
  const int balance = node->left->right->balance;

  node->left = rotateLeft(node->left);
  node = rotateRight(node);

  node->balance = 0;
  node->left->balance = balance == 1 ? -1 : 0;
  node->right->balance = balance == -1 ? 1 : 0;

  return node;
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *rotateRightRight(struct TreeNode *node)
{
  const int balance = node->left->balance;

  node = rotateRight(node);

  node->balance = balance + 1;
  node->right->balance = -1 - balance;

  return node;
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *rotateRightLeft(struct TreeNode *node)
{
  const int balance = node->right->left->balance;

  node->right = rotateRight(node->right);
  node = rotateLeft(node);

  node->balance = 0;
  node->left->balance = balance == 1 ? -1 : 0;
  node->right->balance = balance == -1 ? 1 : 0;

  return node;
}
/*----------------------------------------------------------------------------*/
bool treeInit(struct Tree *tree, size_t width, size_t capacity,
    int (*comparator)(const void *, const void *))
{
  tree->compare = comparator;
  tree->root = nullptr;
  tree->width = width;

  if (capacity)
  {
    const size_t totalNodeSize = getNodeSize(width);

    tree->data = malloc(capacity * totalNodeSize);
    if (tree->data == nullptr)
      return false;
    tree->pool = tree->data;

    makeNodeList(tree, totalNodeSize, capacity);
  }
  else
  {
    tree->data = nullptr;
    tree->pool = nullptr;
  }

  return true;
}
/*----------------------------------------------------------------------------*/
void treeInitArena(struct Tree *tree, size_t width, size_t capacity,
    int (*comparator)(const void *, const void *), void *arena)
{
  tree->compare = comparator;
  tree->data = arena;
  tree->pool = capacity ? arena : nullptr;
  tree->root = nullptr;
  tree->width = width;

  makeNodeList(tree, getNodeSize(width), capacity);
}
/*----------------------------------------------------------------------------*/
void treeDeinit(struct Tree *tree)
{
  if (tree->data != nullptr)
    free(tree->data);
  else
    treeClear(tree);
}
/*----------------------------------------------------------------------------*/
void treeDeinitArena(struct Tree *)
{
}
/*----------------------------------------------------------------------------*/
void treeErase(struct Tree *tree, struct TreeNode *node)
{
  struct TreeNode *child = nullptr;
  struct TreeNode *current = tree->root;
  struct TreeNode *parent = node->parent;
  struct TreeNode *sacrifice = node;
  bool erased = false;

  if (node->right != nullptr && node->left != nullptr)
  {
    parent = node;
    current = node->right;

    while (current->left != nullptr)
    {
      parent = current;
      current = current->left;
    }
    child = current->right;

    memcpy(node->data, current->data, tree->width);
    sacrifice = current;
  }
  else
  {
    current = node;
    sacrifice = node;

    if (node->right != nullptr)
      child = node->right;
    else if (node->left != nullptr)
      child = node->left;
  }

  while (parent != nullptr)
  {
    const int oldBalance = parent->balance;
    const int newBalance = oldBalance + (parent->left == current ? 1 : -1);
    struct TreeNode * const ancestor = parent->parent;
    struct TreeNode *root = nullptr;
    bool propagate = false;

    if (!erased)
    {
      erased = true;

      if (child != nullptr)
        child->parent = parent;

      if (parent->left == current)
        parent->left = child;
      else
        parent->right = child;
    }

    if (newBalance <= -2)
    {
      /* Left heavy */
      if (parent->left->balance >= 1)
        root = rotateLeftRight(parent);
      else
        root = rotateRightRight(parent);
    }
    else if (newBalance >= 2)
    {
      /* Right heavy */
      if (parent->right->balance <= -1)
        root = rotateRightLeft(parent);
      else
        root = rotateLeftLeft(parent);
    }
    else
    {
      if (!newBalance)
        propagate = true;
      parent->balance = newBalance;
    }

    if (root != nullptr)
    {
      if (ancestor != nullptr)
      {
        if (oldBalance && !root->balance)
          propagate = true;

        if (ancestor->left == parent)
          ancestor->left = root;
        else
          ancestor->right = root;
        parent = root;
      }
      else
      {
        tree->root = root;
        break;
      }
    }

    if (!propagate)
      break;

    current = parent;
    parent = ancestor;
  }

  if (sacrifice == tree->root)
  {
    if (child != nullptr)
      child->parent = nullptr;
    tree->root = child;
  }

  nodeRelease(tree, sacrifice);
}
/*----------------------------------------------------------------------------*/
bool treeInsert(struct Tree *tree, const void *element)
{
  struct TreeNode * const node = nodeAllocate(tree);

  if (node != nullptr)
  {
    node->parent = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->balance = 0;

    memcpy(node->data, element, tree->width);
    insertNode(tree, node);

    return true;
  }
  else
    return false;
}
/*----------------------------------------------------------------------------*/
struct TreeNode *treeFind(struct Tree *tree, const void *element)
{
  struct TreeNode *current = tree->root;

  while (current != nullptr)
  {
    const int difference = tree->compare(element, current->data);

    if (difference < 0)
      current = current->left;
    else if (difference > 0)
      current = current->right;
    else
      break;
  }

  return current;
}
/*----------------------------------------------------------------------------*/
void treeClear(struct Tree *tree)
{
  if (tree->root == nullptr)
    return;

  struct TreeNode *current = fetchFirst(tree->root);

  while (current->right != nullptr)
    current = current->right;

  while (current != nullptr)
  {
    struct TreeNode * const previous = current;

    current = fetchNextPostOrder(current);
    nodeRelease(tree, previous);
  }

  tree->root = nullptr;
}
/*----------------------------------------------------------------------------*/
size_t treeSize(const struct Tree *tree)
{
  if (tree->root == nullptr)
    return 0;

  struct TreeNode *current = fetchFirst(tree->root);
  size_t count = 0;

  while (current != nullptr)
  {
    current = fetchNextInOrder(current);
    ++count;
  }

  return count;
}
