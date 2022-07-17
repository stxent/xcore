/*
 * main.c
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

#include "tfs.h"
#include <xcore/fs/utils.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#define MAX_BUFFER_LENGTH 128
#define MAX_FILE_LENGTH   1024
/*----------------------------------------------------------------------------*/
static const char PATH_BOOT[] = "/boot";
static const char PATH_HOME[] = "/home";
static const char PATH_SYS[] = "/sys";

static const char PATH_HOME_ROOT[] = "/home/root";
static const char PATH_HOME_USER[] = "/home/user";
static const char PATH_HOME_RESERVED[] = "/home/xx";

static const char PATH_HOME_USER_FILE[] = "/home/user/file.txt";
static const char PATH_HOME_USER_IMAGE[] = "/home/user/image.bmp";
/*----------------------------------------------------------------------------*/
static void freeNode(struct FsHandle *, const char *);
static void freeTestHandle(struct FsHandle *);
static void makeNode(struct FsHandle *, const char *);
static struct FsHandle *makeTestHandle(void);
/*----------------------------------------------------------------------------*/
extern void *__libc_malloc(size_t);

static unsigned int mallocHookFails = 0;

void *malloc(size_t size)
{
  bool allocate = true;

  if (mallocHookFails && !--mallocHookFails)
    allocate = false;

  return allocate ? __libc_malloc(size) : 0;
}
/*----------------------------------------------------------------------------*/
static void freeNode(struct FsHandle *handle, const char *path)
{
  struct FsNode * const root = fsOpenBaseNode(handle, path);
  ck_assert_ptr_nonnull(root);

  struct FsNode * const node = fsOpenNode(handle, path);
  ck_assert_ptr_nonnull(node);
  const enum Result res = fsNodeRemove(root, node);
  ck_assert_uint_eq(res, E_OK);

  fsNodeFree(node);
  fsNodeFree(root);
}
/*----------------------------------------------------------------------------*/
static void freeTestHandle(struct FsHandle *handle)
{
  /* Level 3 inside "/home/user directory", intentionally reordered */
  freeNode(handle, PATH_HOME_USER_FILE);
  freeNode(handle, PATH_HOME_USER_IMAGE);

  /* Level 2 inside "/home" */
  freeNode(handle, PATH_HOME_RESERVED);
  freeNode(handle, PATH_HOME_ROOT);
  freeNode(handle, PATH_HOME_USER);

  /* Level 1 inside "/" */
  freeNode(handle, PATH_SYS);
  freeNode(handle, PATH_HOME);
  freeNode(handle, PATH_BOOT);

  deinit(handle);
}
/*----------------------------------------------------------------------------*/
static void makeNode(struct FsHandle *handle, const char *path)
{
  static const FsAccess access = FS_ACCESS_READ | FS_ACCESS_WRITE;

  const char * const name = fsExtractName(path);
  ck_assert_ptr_nonnull(name);

  struct FsNode * const parent = fsOpenBaseNode(handle, path);
  ck_assert_ptr_nonnull(parent);

  const struct FsFieldDescriptor desc[] = {
      {
          name,
          strlen(name) + 1,
          FS_NODE_NAME
      }, {
          &access,
          sizeof(access),
          FS_NODE_ACCESS
      }
  };
  const enum Result res = fsNodeCreate(parent, desc, ARRAY_SIZE(desc));
  ck_assert_uint_eq(res, E_OK);
  fsNodeFree(parent);

  struct FsNode *node = fsOpenNode(handle, path);
  ck_assert_ptr_nonnull(node);
  fsNodeFree(node);
}
/*----------------------------------------------------------------------------*/
static struct FsHandle *makeTestHandle(void)
{
  struct FsHandle * const handle = init(TfsHandle, 0);

  /* Level 1 inside "/" */
  makeNode(handle, PATH_BOOT);
  makeNode(handle, PATH_HOME);
  makeNode(handle, PATH_SYS);

  /* Level 2 inside "/home" */
  makeNode(handle, PATH_HOME_ROOT);
  makeNode(handle, PATH_HOME_USER);
  makeNode(handle, PATH_HOME_RESERVED);

  /* Level 3 inside "/home/user directory" */
  makeNode(handle, PATH_HOME_USER_FILE);
  makeNode(handle, PATH_HOME_USER_IMAGE);

  return handle;
}
/*----------------------------------------------------------------------------*/
START_TEST(testAccess)
{
  struct FsHandle * const handle = makeTestHandle();

  struct FsNode * const parent = fsOpenBaseNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(parent);
  struct FsNode * const subnode = fsOpenNode(handle, PATH_HOME_USER_IMAGE);
  ck_assert_ptr_nonnull(subnode);

  enum Result res;
  FsAccess access;
  size_t count;

  /* Try to write incorrect buffer */

  access = FS_ACCESS_READ | FS_ACCESS_WRITE;
  res = fsNodeWrite(parent, FS_NODE_ACCESS, 0, &access, 1, &count);
  ck_assert_uint_eq(res, E_VALUE);

  /* Change node access */

  access = 0;
  res = fsNodeWrite(parent, FS_NODE_ACCESS, 0, &access, sizeof(access), &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(count, sizeof(access));

  /* Read node access */

  access = FS_ACCESS_READ | FS_ACCESS_WRITE;
  res = fsNodeRead(parent, FS_NODE_ACCESS, 0, &access, sizeof(access), &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(count, sizeof(access));
  ck_assert_uint_eq(access, 0);

  /* Try to open a node inside the forbidden directory */

  struct FsNode * const node = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_null(node);

  /* Try to create a node inside the forbidden directory */

  const struct FsFieldDescriptor desc[] = {
      {
          "test",
          strlen("test") + 1,
          FS_NODE_NAME
      }
  };
  res = fsNodeCreate(parent, desc, ARRAY_SIZE(desc));
  ck_assert_uint_eq(res, E_ACCESS);

  /* Try to remove a node from the forbidden directory */

  res = fsNodeRemove(parent, subnode);
  ck_assert_uint_eq(res, E_ACCESS);

  /* Restore access */

  access = FS_ACCESS_READ | FS_ACCESS_WRITE;
  res = fsNodeWrite(parent, FS_NODE_ACCESS, 0, &access, sizeof(access), 0);
  ck_assert_uint_eq(res, E_OK);

  /* Change subnode access */

  access = FS_ACCESS_READ;
  res = fsNodeWrite(subnode, FS_NODE_ACCESS, 0, &access, sizeof(access), 0);
  ck_assert_uint_eq(res, E_OK);

  /* Try to remove a read-only node */

  res = fsNodeRemove(parent, subnode);
  ck_assert_uint_eq(res, E_ACCESS);

  /* Restore subnode access */

  access = FS_ACCESS_READ | FS_ACCESS_WRITE;
  res = fsNodeWrite(subnode, FS_NODE_ACCESS, 0, &access, sizeof(access), 0);
  ck_assert_uint_eq(res, E_OK);

  fsNodeFree(subnode);
  fsNodeFree(parent);
  freeTestHandle(handle);
}
/*----------------------------------------------------------------------------*/
START_TEST(testAccessIter)
{
  struct FsHandle * const handle = makeTestHandle();

  struct FsNode * const parent = fsOpenBaseNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(parent);

  struct FsNode *node = fsNodeHead(parent);
  ck_assert_ptr_nonnull(node);

  enum Result res;
  FsAccess access;

  /* Change node access */

  access = 0;
  res = fsNodeWrite(parent, FS_NODE_ACCESS, 0, &access, sizeof(access), 0);
  ck_assert_uint_eq(res, E_OK);

  /* Try to fetch next node */

  res = fsNodeNext(node);
  ck_assert_uint_eq(res, E_ACCESS);

  /* Restore access */

  access = FS_ACCESS_READ | FS_ACCESS_WRITE;
  res = fsNodeWrite(parent, FS_NODE_ACCESS, 0, &access, sizeof(access), 0);
  ck_assert_uint_eq(res, E_OK);

  fsNodeFree(node);
  fsNodeFree(parent);
  freeTestHandle(handle);
}
/*----------------------------------------------------------------------------*/
START_TEST(testAccessReadWrite)
{
  struct FsHandle * const handle = makeTestHandle();

  struct FsNode * const node = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(node);

  char buffer[MAX_BUFFER_LENGTH] = {0};
  enum Result res;
  FsAccess access;

  /* Change node access */

  access = 0;
  res = fsNodeWrite(node, FS_NODE_ACCESS, 0, &access, sizeof(access), 0);
  ck_assert_uint_eq(res, E_OK);

  /* Try to write data */

  res = fsNodeWrite(node, FS_NODE_DATA, 0, buffer, sizeof(buffer), 0);
  ck_assert_uint_eq(res, E_ACCESS);

  /* Try to read data */

  res = fsNodeRead(node, FS_NODE_DATA, 0, buffer, sizeof(buffer), 0);
  ck_assert_uint_eq(res, E_ACCESS);

  /* Restore access */

  access = FS_ACCESS_READ | FS_ACCESS_WRITE;
  res = fsNodeWrite(node, FS_NODE_ACCESS, 0, &access, sizeof(access), 0);
  ck_assert_uint_eq(res, E_OK);

  fsNodeFree(node);
  freeTestHandle(handle);
}
/*----------------------------------------------------------------------------*/
START_TEST(testNodeCapacity)
{
  struct FsHandle * const handle = makeTestHandle();
  enum Result res;

  /* Prepare data file */

  struct FsNode * const node = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(node);

  for (size_t i = 0; i < MAX_FILE_LENGTH / MAX_BUFFER_LENGTH; ++i)
  {
    size_t count;
    char pattern[MAX_BUFFER_LENGTH];

    memset(pattern, i, MAX_BUFFER_LENGTH);

    count = 0;
    res = fsNodeWrite(node, FS_NODE_DATA, i * MAX_BUFFER_LENGTH,
        pattern, MAX_BUFFER_LENGTH, &count);
    ck_assert_uint_eq(res, E_OK);
    ck_assert_uint_eq(count, MAX_BUFFER_LENGTH);
  }

  /* Read capacity */

  FsCapacity capacity;
  size_t count;

  res = fsNodeRead(node, FS_NODE_CAPACITY, 0, &capacity, sizeof(capacity),
      &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(capacity, MAX_FILE_LENGTH);
  ck_assert_uint_eq(count, sizeof(capacity));

  /* Read capacity failure */

  res = fsNodeRead(node, FS_NODE_CAPACITY, 0, &capacity, 0, 0);
  ck_assert_uint_eq(res, E_VALUE);

  fsNodeFree(node);
  freeTestHandle(handle);
}
/*----------------------------------------------------------------------------*/
START_TEST(testNodeLength)
{
  static const char data[MAX_BUFFER_LENGTH] = {0};

  struct FsHandle * const handle = makeTestHandle();
  FsLength count;
  enum Result res;

  /* Standard file */

  struct FsNode * const node = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(node);

  res = fsNodeWrite(node, FS_NODE_DATA, 0, data, MAX_BUFFER_LENGTH, 0);
  ck_assert_uint_eq(res, E_OK);

  count = 0;
  res = fsNodeLength(node, FS_NODE_ACCESS, &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(count, sizeof(FsAccess));

  count = 0;
  res = fsNodeLength(node, FS_NODE_CAPACITY, &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(count, sizeof(FsCapacity));

  count = 0;
  res = fsNodeLength(node, FS_NODE_DATA, &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(count, MAX_BUFFER_LENGTH);

  count = 0;
  res = fsNodeLength(node, FS_NODE_NAME, &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(count, strlen(fsExtractName(PATH_HOME_USER_FILE)) + 1);

  res = fsNodeLength(node, FS_NODE_ID, &count);
  ck_assert_uint_eq(res, E_INVALID);

  fsNodeFree(node);

  /* Root node */

  struct FsNode * const root = fsOpenNode(handle, "/");
  ck_assert_ptr_nonnull(root);

  res = fsNodeLength(root, FS_NODE_NAME, &count);
  ck_assert_uint_eq(res, E_INVALID);

  fsNodeFree(root);

  /* Release all resources */
  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testNodeRenaming)
{
  struct FsHandle * const handle = makeTestHandle();
  struct FsNode * const node = fsOpenNode(handle, PATH_HOME_RESERVED);
  ck_assert_ptr_nonnull(node);

  size_t written;
  enum Result res;

  /* Rename to a reserved name */
  res = fsNodeWrite(node, FS_NODE_NAME, 0, "..", 3, &written);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(written, 3);

  /* Renaming failure */
  mallocHookFails = 1;
  res = fsNodeWrite(node, FS_NODE_NAME, 0, "xx", 3, &written);
  ck_assert_uint_eq(res, E_MEMORY);

  /* Incorrect position */
  res = fsNodeWrite(node, FS_NODE_NAME, 1, "x", 2, &written);
  ck_assert_uint_eq(res, E_VALUE);

  /* Name length does not match buffer content */
  res = fsNodeWrite(node, FS_NODE_NAME, 0, "x", 3, &written);
  ck_assert_uint_eq(res, E_VALUE);

  /* Erase name */
  res = fsNodeWrite(node, FS_NODE_NAME, 0, 0, 0, &written);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(written, 0);

  /* Restore original name */
  written = 0;
  res = fsNodeWrite(node, FS_NODE_NAME, 0, "xx", 3, &written);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(written, 3);

  fsNodeFree(node);
  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPartExtraction)
{
  struct FsHandle * const handle = makeTestHandle();
  struct FsNode *node;
  struct FsNode *tmp;
  const char *path;

  /* Empty path */
  node = 0;
  path = fsFollowNextPart(handle, &node, "", true);
  ck_assert_ptr_null(path);
  ck_assert_ptr_null(node);

  /* Forbidden strings */
  node = 0;
  path = fsFollowNextPart(handle, &node, "/..", true);
  ck_assert_ptr_nonnull(path);
  ck_assert_ptr_nonnull(node);
  tmp = node;
  path = fsFollowNextPart(handle, &node, path, true);
  ck_assert_ptr_null(path);
  ck_assert_ptr_eq(node, tmp); /* Node must be left untouched */

  node = 0;
  path = fsFollowNextPart(handle, &node, "/.", true);
  ck_assert_ptr_nonnull(path);
  ck_assert_ptr_nonnull(node);
  tmp = node;
  path = fsFollowNextPart(handle, &node, path, true);
  ck_assert_ptr_null(path);
  ck_assert_ptr_eq(node, tmp); /* Node must be left untouched */

  /* Incorrect call sequence */
  node = 0;
  path = fsFollowNextPart(handle, &node, "home", true);
  ck_assert_ptr_null(path);
  ck_assert_ptr_null(node);

  /* Non-existent node */
  node = 0;
  path = fsFollowNextPart(handle, &node, "/tmp", true);
  ck_assert_ptr_nonnull(path);
  ck_assert_ptr_nonnull(node);
  tmp = node;
  path = fsFollowNextPart(handle, &node, path, true);
  ck_assert_ptr_null(path);
  ck_assert_ptr_eq(node, tmp); /* Node must be left untouched */

  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPathFollowing)
{
  const char * const name = fsExtractName(PATH_HOME_USER_IMAGE);
  struct FsHandle * const handle = makeTestHandle();
  enum Result res;

  struct FsNode * const dirNode = fsOpenBaseNode(handle, PATH_HOME_USER_IMAGE);
  ck_assert_ptr_nonnull(dirNode);

  /*
   * Virtual directory has a LIFO structure. "FILE" node should be created
   * before "IMAGE" node to be placed in the end of the list.
   */
  struct FsNode * const imgNode = fsOpenNode(handle, PATH_HOME_USER_IMAGE);
  ck_assert_ptr_nonnull(imgNode);
  ck_assert_ptr_ne(dirNode, imgNode);

  /* Simulate name reading failure */
  res = fsNodeWrite(imgNode, FS_NODE_NAME, 0, "\x7F", 2, 0);
  ck_assert_uint_eq(res, E_OK);

  struct FsNode * const txtNode = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(txtNode);
  ck_assert_ptr_ne(dirNode, txtNode);
  fsNodeFree(txtNode);

  /* Restore original node name and free node */
  res = fsNodeWrite(imgNode, FS_NODE_NAME, 0, name, strlen(name) + 1, 0);
  ck_assert_uint_eq(res, E_OK);
  fsNodeFree(imgNode);

  fsNodeFree(dirNode);
  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testReadWrite)
{
  struct FsHandle * const handle = makeTestHandle();
  char buffer[MAX_BUFFER_LENGTH];
  enum Result res;

  /* Standard file */

  struct FsNode * const node = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(node);

  for (size_t i = 0; i < MAX_FILE_LENGTH / MAX_BUFFER_LENGTH; ++i)
  {
    size_t count;
    char pattern[MAX_BUFFER_LENGTH];

    memset(pattern, i, MAX_BUFFER_LENGTH);

    count = 0;
    res = fsNodeWrite(node, FS_NODE_DATA, i * MAX_BUFFER_LENGTH,
        pattern, MAX_BUFFER_LENGTH, &count);
    ck_assert_uint_eq(res, E_OK);
    ck_assert_uint_eq(count, MAX_BUFFER_LENGTH);

    count = 0;
    res = fsNodeRead(node, FS_NODE_DATA, i * MAX_BUFFER_LENGTH,
        buffer, MAX_BUFFER_LENGTH, &count);
    ck_assert_uint_eq(res, E_OK);
    ck_assert_uint_eq(count, MAX_BUFFER_LENGTH);
    ck_assert_mem_eq(buffer, pattern, MAX_BUFFER_LENGTH);
  }

  memset(buffer, 0, sizeof(buffer));
  res = fsNodeWrite(node, FS_TYPE_END, 0, buffer, sizeof(buffer), 0);
  ck_assert_uint_eq(res, E_INVALID);

  res = fsNodeRead(node, FS_NODE_DATA, MAX_FILE_LENGTH + 1,
      buffer, sizeof(buffer), 0);
  ck_assert_uint_eq(res, E_VALUE);

  fsNodeFree(node);

  /* Root node */

  struct FsNode * const root = fsOpenNode(handle, "/");
  ck_assert_ptr_nonnull(root);

  res = fsNodeRead(node, FS_NODE_NAME, 0, buffer, MAX_BUFFER_LENGTH, 0);
  ck_assert_uint_eq(res, E_INVALID);

  fsNodeFree(root);

  /* Release all resources */
  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testTfsErrors)
{
  struct FsHandle * const handle = makeTestHandle();
  struct FsNode *node;
  struct FsNode *parent;
  enum Result res;

  /* Try to open a child of empty node */

  parent = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(parent);
  node = fsNodeHead(parent);
  ck_assert_ptr_null(node);
  fsNodeFree(parent);

  /* Try to remove nondescendant node */

  parent = fsOpenNode(handle, PATH_HOME);
  ck_assert_ptr_nonnull(parent);
  node = fsOpenNode(handle, PATH_BOOT);
  ck_assert_ptr_nonnull(node);
  res = fsNodeRemove(parent, node);
  ck_assert_uint_eq(res, E_ENTRY);
  fsNodeFree(node);
  fsNodeFree(parent);

  /* Try to read unavailable attributes */

  node = fsOpenNode(handle, PATH_HOME);
  ck_assert_ptr_nonnull(node);

  char buffer[MAX_BUFFER_LENGTH];
  size_t count;

  res = fsNodeRead(node, FS_NODE_ACCESS, 1, buffer, sizeof(buffer), 0);
  ck_assert_uint_eq(res, E_VALUE);
  res = fsNodeRead(node, FS_NODE_TIME, 0, buffer, sizeof(buffer), 0);
  ck_assert_uint_eq(res, E_INVALID);

  res = fsNodeRead(node, FS_NODE_NAME, 0, buffer, sizeof(buffer), &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_str_eq(buffer, "home");
  ck_assert_uint_eq(count, strlen("home") + 1);

  res = fsNodeRead(node, FS_NODE_NAME, 0, buffer, sizeof(buffer), 0);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_str_eq(buffer, "home");

  res = fsNodeRead(node, FS_NODE_NAME, 1, buffer, sizeof(buffer), 0);
  ck_assert_uint_eq(res, E_VALUE);
  res = fsNodeRead(node, FS_NODE_NAME, 0, buffer, 1, 0);
  ck_assert_uint_eq(res, E_VALUE);

  fsNodeFree(node);

  /* Iterate over handle head */

  parent = fsOpenNode(handle, "/");
  ck_assert_ptr_nonnull(parent);
  res = fsNodeNext(parent);
  ck_assert_uint_eq(res, E_ENTRY);
  fsNodeFree(parent);

  /* Iterate to the end of directory */

  parent = fsOpenNode(handle, "/");
  ck_assert_ptr_nonnull(parent);
  node = fsNodeHead(parent);
  ck_assert_ptr_nonnull(node);
  res = fsNodeNext(node);
  ck_assert_uint_eq(res, E_OK);
  res = fsNodeNext(node);
  ck_assert_uint_eq(res, E_OK);
  res = fsNodeNext(node);
  ck_assert_uint_eq(res, E_ENTRY);
  fsNodeFree(node);
  fsNodeFree(parent);

  /* Sync handle */

  fsHandleSync(handle);

  /* Create incorrect nodes */

  parent = fsOpenNode(handle, PATH_HOME);
  ck_assert_ptr_nonnull(parent);

  const struct FsFieldDescriptor incorrectNodeDesc[] = {
      {
          "error",
          strlen("error"),
          FS_NODE_NAME
      }
  };

  res = fsNodeCreate(parent, incorrectNodeDesc, 0);
  ck_assert_uint_eq(res, E_VALUE);

  res = fsNodeCreate(parent, incorrectNodeDesc, ARRAY_SIZE(incorrectNodeDesc));
  ck_assert_uint_eq(res, E_VALUE);

  fsNodeFree(parent);

  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testTfsMemoryErrors)
{
  static const char data[MAX_BUFFER_LENGTH] = {0};

  struct FsHandle * const handle = makeTestHandle();
  struct FsNode * const parent = fsOpenNode(handle, PATH_HOME);
  ck_assert_ptr_nonnull(parent);

  const struct FsFieldDescriptor incorrectNodeDesc[] = {
      {
          "error",
          strlen("error") + 1,
          FS_NODE_NAME
      }
  };
  enum Result res;

  mallocHookFails = 1;
  res = fsNodeCreate(parent, incorrectNodeDesc, 0);
  ck_assert_uint_eq(res, E_VALUE);

  mallocHookFails = 2;
  res = fsNodeCreate(parent, incorrectNodeDesc,
      ARRAY_SIZE(incorrectNodeDesc));
  ck_assert_uint_eq(res, E_MEMORY);

  mallocHookFails = 3;
  res = fsNodeCreate(parent, incorrectNodeDesc,
      ARRAY_SIZE(incorrectNodeDesc));
  ck_assert_uint_eq(res, E_MEMORY);

  struct FsNode * const node = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(node);
  mallocHookFails = 1;
  res = fsNodeWrite(node, FS_NODE_DATA, 0, data, MAX_BUFFER_LENGTH, 0);
  ck_assert_uint_eq(res, E_MEMORY);
  fsNodeFree(node);

  fsNodeFree(parent);
  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testUsedSpaceCalculation)
{
  static const size_t MAX_FILE_SIZE = 16384;
  static const char data[MAX_BUFFER_LENGTH] = {0};

  struct FsHandle * const handle = makeTestHandle();
  struct FsNode *node;
  enum Result res;
  FsCapacity used;

  /* Node with full capacity usage */

  node = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(node);

  for (size_t i = 0; i < MAX_FILE_SIZE / MAX_BUFFER_LENGTH; ++i)
  {
    res = fsNodeWrite(node, FS_NODE_DATA, (FsLength)(i * MAX_BUFFER_LENGTH),
        data, MAX_BUFFER_LENGTH, 0);
    ck_assert_uint_eq(res, E_OK);
  }

  fsNodeFree(node);

  used = fsFindUsedSpace(handle, 0);
  ck_assert_uint_eq(used, MAX_FILE_SIZE);

  /* Node with partial capacity usage */

  node = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(node);

  for (size_t i = 0; i < (MAX_FILE_SIZE * 3) / (MAX_BUFFER_LENGTH * 4); ++i)
  {
    res = fsNodeWrite(node, FS_NODE_DATA, (FsLength)(i * MAX_BUFFER_LENGTH),
        data, MAX_BUFFER_LENGTH, 0);
    ck_assert_uint_eq(res, E_OK);
  }

  fsNodeFree(node);

  used = fsFindUsedSpace(handle, 0);
  ck_assert_uint_eq(used, MAX_FILE_SIZE);

  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testUsedSpaceErrors)
{
  static const size_t MAX_FILE_SIZE = 16384;
  static const char data[MAX_BUFFER_LENGTH] = {0};

  struct FsHandle * const handle = makeTestHandle();
  struct FsNode *node;
  enum Result res;

  /* Make node with filled with data */

  node = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(node);

  for (size_t i = 0; i < MAX_FILE_SIZE / MAX_BUFFER_LENGTH; ++i)
  {
    res = fsNodeWrite(node, FS_NODE_DATA, (FsLength)(i * MAX_BUFFER_LENGTH),
        data, MAX_BUFFER_LENGTH, 0);
    ck_assert_uint_eq(res, E_OK);
  }

  fsNodeFree(node);

  /* Test usage */

  node = fsOpenNode(handle, PATH_HOME_RESERVED);
  ck_assert_ptr_nonnull(node);

  FsCapacity used;

  /* Make node with a reserved name */
  res = fsNodeWrite(node, FS_NODE_NAME, 0, "..", 3, 0);
  ck_assert_uint_eq(res, E_OK);
  used = fsFindUsedSpace(handle, 0);
  ck_assert_uint_eq(used, MAX_FILE_SIZE);

  /* Test head allocation failure */
  mallocHookFails = 1;
  used = fsFindUsedSpace(handle, 0);
  ck_assert_uint_eq(used, 0);

  /* Test incorrect name skipping */
  res = fsNodeWrite(node, FS_NODE_NAME, 0, 0, 0, 0);
  ck_assert_uint_eq(res, E_OK);
  used = fsFindUsedSpace(handle, 0);
  ck_assert_uint_eq(used, MAX_FILE_SIZE);

  /* Test incorrect name reading */
  res = fsNodeWrite(node, FS_NODE_NAME, 0, "\x7F", 2, 0);
  ck_assert_uint_eq(res, E_OK);
  used = fsFindUsedSpace(handle, 0);
  ck_assert_uint_eq(used, 0);

  /* Restore original node name */
  fsNodeWrite(node, FS_NODE_NAME, 0, "xx", 3, 0);
  fsNodeFree(node);

  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("TestFileSystem");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testAccess);
  tcase_add_test(testcase, testAccessIter);
  tcase_add_test(testcase, testAccessReadWrite);
  tcase_add_test(testcase, testNodeCapacity);
  tcase_add_test(testcase, testNodeLength);
  tcase_add_test(testcase, testNodeRenaming);
  tcase_add_test(testcase, testPartExtraction);
  tcase_add_test(testcase, testPathFollowing);
  tcase_add_test(testcase, testReadWrite);
  tcase_add_test(testcase, testTfsErrors);
  tcase_add_test(testcase, testTfsMemoryErrors);
  tcase_add_test(testcase, testUsedSpaceCalculation);
  tcase_add_test(testcase, testUsedSpaceErrors);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
