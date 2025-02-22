/*
 * tree.c
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/tree.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
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
static struct TreeNode *fetchFirst(struct TreeNode *node)
{
  struct TreeNode *current = node->left;

  if (current != NULL)
  {
    while (current->left != NULL)
      current = current->left;
  }

  return current;
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *fetchNextInOrder(struct TreeNode *node)
{
  if (node->right != NULL)
  {
    struct TreeNode *current = node->right;

    while (current != NULL && current->left != NULL)
      current = current->left;

    return current;
  }

  struct TreeNode *parent = node->parent;

  while (parent != NULL && parent->right == node)
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

  if (parent == NULL)
    return NULL;

  if (parent->right == node || parent->right == NULL)
    return parent;

  struct TreeNode *current = parent->right;

  while (current != NULL && (current->left != NULL || current->right != NULL))
    current = current->left != NULL ? current->left : current->right;

  return current;
}
/*----------------------------------------------------------------------------*/
static void insertNode(struct Tree *tree, struct TreeNode *node)
{
  struct TreeNode *current = tree->root;
  struct TreeNode *parent = NULL;
  bool left = false;

  while (current != NULL)
  {
    parent = current;

    left = tree->compare(node->data, current->data) < 0;
    current = left ? current->left : current->right;
  }

  if (parent != NULL)
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

  while (parent != NULL)
  {
    const int newBalance = parent->balance + (parent->left == current ? -1 : 1);
    struct TreeNode * const ancestor = parent->parent;
    struct TreeNode *root = NULL;
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

    if (root != NULL)
    {
      if (ancestor != NULL)
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
  if (a->right != NULL)
    a->right->parent = a;

  return b;
}
/*----------------------------------------------------------------------------*/
static struct TreeNode *rotateRight(struct TreeNode *node)
{
  struct TreeNode * const a = node;
  struct TreeNode * const b = a->left;
  const int balance = b->balance;

  a->left = b->right;
  b->right = a;

  b->parent = a->parent;
  a->parent = b;
  b->right->parent = b;
  if (a->left != NULL)
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
void treeInit(struct Tree *tree, size_t width,
    int (*comparator)(const void *, const void *))
{
  tree->compare = comparator;
  tree->root = NULL;
  tree->width = width;
}
/*----------------------------------------------------------------------------*/
void treeDeinit(struct Tree *tree)
{
  treeClear(tree);
}
/*----------------------------------------------------------------------------*/
void treeErase(struct Tree *tree, struct TreeNode *node)
{
  struct TreeNode *child = NULL;
  struct TreeNode *current = tree->root;
  struct TreeNode *parent = node->parent;
  struct TreeNode *sacrifice = node;
  bool erased = false;

  if (node->right != NULL && node->left != NULL)
  {
    parent = node;
    current = node->right;

    while (current->left != NULL)
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

    if (node->right != NULL)
      child = node->right;
    else if (node->left != NULL)
      child = node->left;
  }

  while (parent != NULL)
  {
    const int oldBalance = parent->balance;
    const int newBalance = oldBalance + (parent->left == current ? 1 : -1);
    struct TreeNode * const ancestor = parent->parent;
    struct TreeNode *root = NULL;
    bool propagate = false;

    if (!erased)
    {
      erased = true;

      if (child != NULL)
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

    if (root != NULL)
    {
      if (ancestor != NULL)
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
    if (child != NULL)
      child->parent = NULL;
    tree->root = child;
  }

  free(sacrifice);
}
/*----------------------------------------------------------------------------*/
bool treeInsert(struct Tree *tree, const void *element)
{
  struct TreeNode * const node =
      malloc(offsetof(struct TreeNode, data) + tree->width);

  if (node != NULL)
  {
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
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

  while (current != NULL)
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
  if (tree->root == NULL)
    return;

  struct TreeNode *current = fetchFirst(tree->root);

  while (current->right != NULL)
    current = current->right;

  while (current != NULL)
  {
    struct TreeNode * const previous = current;

    current = fetchNextPostOrder(current);
    free(previous);
  }

  tree->root = NULL;
}
/*----------------------------------------------------------------------------*/
size_t treeSize(const struct Tree *tree)
{
  if (tree->root == NULL)
    return 0;

  struct TreeNode *current = fetchFirst(tree->root);
  size_t count = 0;

  while (current != NULL)
  {
    current = fetchNextInOrder(current);
    ++count;
  }

  return count;
}
