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
  if (mallocHookFails)
  {
    if (--mallocHookFails)
      return __libc_malloc(size);
    else
      return 0;
  }
  else
    return __libc_malloc(size);
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
  /* Level 3 inside "/home/user directory" */
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
  res = fsNodeLength(node, FS_NODE_DATA, &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(count, MAX_BUFFER_LENGTH);

  count = 0;
  res = fsNodeLength(node, FS_NODE_SPACE, &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(count, MAX_BUFFER_LENGTH);

  count = 0;
  res = fsNodeLength(node, FS_NODE_NAME, &count);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(count, strlen(fsExtractName(PATH_HOME_USER_FILE)) + 1);

  res = fsNodeLength(node, FS_NODE_ID, &count);
  ck_assert_uint_ne(res, E_OK);

  fsNodeFree(node);

  /* Root node */

  struct FsNode * const root = fsOpenNode(handle, "/");
  ck_assert_ptr_nonnull(root);

  res = fsNodeLength(root, FS_NODE_NAME, &count);
  ck_assert_uint_ne(res, E_OK);

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
  ck_assert_uint_ne(res, E_OK);

  /* Incorrect position */
  res = fsNodeWrite(node, FS_NODE_NAME, 1, "x", 2, &written);
  ck_assert_uint_ne(res, E_OK);

  /* Name length does not match buffer content */
  res = fsNodeWrite(node, FS_NODE_NAME, 0, "x", 3, &written);
  ck_assert_uint_ne(res, E_OK);

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

  /* Empty path */
  node = 0;
  const char * const res0 = fsFollowNextPart(handle, &node, "", true);
  ck_assert_ptr_null(res0);
  ck_assert_ptr_null(node);

  /* Forbidden strings */
  node = 0;
  const char * const res1A = fsFollowNextPart(handle, &node, "/..", true);
  ck_assert_ptr_nonnull(res1A);
  ck_assert_ptr_nonnull(node);
  tmp = node;
  const char * const res1B = fsFollowNextPart(handle, &node, res1A, true);
  ck_assert_ptr_null(res1B);
  ck_assert_ptr_eq(node, tmp); /* Node must be left untouched */

  node = 0;
  const char * const res2A = fsFollowNextPart(handle, &node, "/.", true);
  ck_assert_ptr_nonnull(res2A);
  ck_assert_ptr_nonnull(node);
  tmp = node;
  const char * const res2B = fsFollowNextPart(handle, &node, res2A, true);
  ck_assert_ptr_null(res2B);
  ck_assert_ptr_eq(node, tmp); /* Node must be left untouched */

  /* Incorrect call sequence */
  node = 0;
  const char * const res3 = fsFollowNextPart(handle, &node, "home", true);
  ck_assert_ptr_null(res3);
  ck_assert_ptr_null(node);

  /* Non-existent node */
  node = 0;
  const char * const res4A = fsFollowNextPart(handle, &node, "/tmp", true);
  ck_assert_ptr_nonnull(res4A);
  ck_assert_ptr_nonnull(node);
  tmp = node;
  const char * const res4B = fsFollowNextPart(handle, &node, res4A, true);
  ck_assert_ptr_null(res4B);
  ck_assert_ptr_eq(node, tmp); /* Node must be left untouched */

  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPathFollowing)
{
  struct FsHandle * const handle = makeTestHandle();

  struct FsNode * const dirNode = fsOpenBaseNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(dirNode);
  struct FsNode * const txtNode = fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(txtNode);
  ck_assert_ptr_ne(dirNode, txtNode);

  fsNodeFree(dirNode);
  fsNodeFree(txtNode);

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
  res = fsNodeWrite(node, FS_TYPE_END, 0,
      buffer, sizeof(buffer), 0);
  ck_assert_uint_ne(res, E_OK);

  res = fsNodeRead(node, FS_NODE_DATA, MAX_FILE_LENGTH + 1,
      buffer, sizeof(buffer), 0);
  ck_assert_uint_ne(res, E_OK);

  fsNodeFree(node);

  /* Root node */

  struct FsNode * const root = fsOpenNode(handle, "/");
  ck_assert_ptr_nonnull(root);

  res = fsNodeRead(node, FS_NODE_NAME, 0, buffer, MAX_BUFFER_LENGTH, 0);
  ck_assert_uint_ne(res, E_OK);

  fsNodeFree(root);

  /* Release all resources */
  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testTfsErrors)
{
  struct FsHandle * const handle = makeTestHandle();

  /* Try to open a child of empty node */
  struct FsNode * const emptyNode =
      fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(emptyNode);
  struct FsNode * const emptyNodeChild = fsNodeHead(emptyNode);
  ck_assert_ptr_null(emptyNodeChild);
  fsNodeFree(emptyNode);

  /* Try to remove nondescendant node */
  struct FsNode * const nondescNodeA = fsOpenNode(handle, PATH_HOME);
  ck_assert_ptr_nonnull(nondescNodeA);
  struct FsNode * const nondescNodeB = fsOpenNode(handle, PATH_BOOT);
  ck_assert_ptr_nonnull(nondescNodeB);
  const enum Result nondescRes = fsNodeRemove(nondescNodeA, nondescNodeB);
  ck_assert_uint_eq(nondescRes, E_ENTRY);
  fsNodeFree(nondescNodeA);
  fsNodeFree(nondescNodeB);

  /* Try to read unavailable atttributes */
  struct FsNode * const attrNode = fsOpenNode(handle, PATH_HOME);
  ck_assert_ptr_nonnull(attrNode);
  char buffer[MAX_BUFFER_LENGTH];
  size_t count;

  const enum Result attrNodeNameResA =
      fsNodeRead(attrNode, FS_NODE_NAME, 0, buffer, sizeof(buffer), &count);
  ck_assert_uint_eq(attrNodeNameResA, E_OK);
  ck_assert_str_eq(buffer, "home");
  ck_assert_uint_eq(count, strlen("home") + 1);
  const enum Result attrNodeNameResB =
      fsNodeRead(attrNode, FS_NODE_NAME, 0, buffer, sizeof(buffer), 0);
  ck_assert_uint_eq(attrNodeNameResB, E_OK);
  ck_assert_str_eq(buffer, "home");
  const enum Result attrNodeNameResC =
      fsNodeRead(attrNode, FS_NODE_NAME, 1, buffer, sizeof(buffer), 0);
  ck_assert_uint_ne(attrNodeNameResC, E_OK);
  const enum Result attrNodeNameResD =
      fsNodeRead(attrNode, FS_NODE_NAME, 0, buffer, 1, 0);
  ck_assert_uint_ne(attrNodeNameResD, E_OK);
  const enum Result attrNodeNameResE =
      fsNodeRead(attrNode, FS_NODE_ACCESS, 0, buffer, sizeof(buffer), 0);
  ck_assert_uint_ne(attrNodeNameResE, E_OK);

  fsNodeFree(attrNode);

  /* Iterate over handle head */
  struct FsNode * const headNode = fsOpenNode(handle, "/");
  ck_assert_ptr_nonnull(headNode);
  const enum Result headNodeRes = fsNodeNext(headNode);
  ck_assert_uint_eq(headNodeRes, E_ENTRY);
  fsNodeFree(headNode);

  /* Iterate to the end of directory */
  struct FsNode * const endNode = fsOpenNode(handle, "/");
  ck_assert_ptr_nonnull(headNode);
  struct FsNode * const endNodeChild = fsNodeHead(endNode);
  ck_assert_ptr_nonnull(endNodeChild);
  const enum Result endNode2Res = fsNodeNext(endNodeChild);
  ck_assert_uint_eq(endNode2Res, E_OK);
  const enum Result endNode3Res = fsNodeNext(endNodeChild);
  ck_assert_uint_eq(endNode3Res, E_OK);
  const enum Result endNode4Res = fsNodeNext(endNodeChild);
  ck_assert_uint_eq(endNode4Res, E_ENTRY);
  fsNodeFree(endNode);
  fsNodeFree(endNodeChild);

  /* Sync handle */
  fsHandleSync(handle);

  /* Create incorrect nodes */
  struct FsNode * const incorrectNodeParent = fsOpenNode(handle, PATH_HOME);
  ck_assert_ptr_nonnull(incorrectNodeParent);

  const struct FsFieldDescriptor incorrectNodeDesc[] = {
      {
          "error",
          strlen("error"),
          FS_NODE_NAME
      }
  };

  const enum Result incorrectNodeResA = fsNodeCreate(incorrectNodeParent,
      incorrectNodeDesc, 0);
  ck_assert_uint_ne(incorrectNodeResA, E_OK);

  const enum Result incorrectNodeResB = fsNodeCreate(incorrectNodeParent,
      incorrectNodeDesc, ARRAY_SIZE(incorrectNodeDesc));
  ck_assert_uint_ne(incorrectNodeResB, E_OK);

  fsNodeFree(incorrectNodeParent);

  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testTfsMemoryErrors)
{
  static const char data[MAX_BUFFER_LENGTH] = {0};

  struct FsHandle * const handle = makeTestHandle();
  struct FsNode * const incorrectNodeParent = fsOpenNode(handle, PATH_HOME);
  ck_assert_ptr_nonnull(incorrectNodeParent);
  enum Result res;

  const struct FsFieldDescriptor incorrectNodeDesc[] = {
      {
          "error",
          strlen("error") + 1,
          FS_NODE_NAME
      }
  };

  mallocHookFails = 1;
  res = fsNodeCreate(incorrectNodeParent, incorrectNodeDesc, 0);
  ck_assert_uint_ne(res, E_OK);

  mallocHookFails = 2;
  res = fsNodeCreate(incorrectNodeParent, incorrectNodeDesc,
      ARRAY_SIZE(incorrectNodeDesc));
  ck_assert_uint_ne(res, E_OK);

  mallocHookFails = 3;
  res = fsNodeCreate(incorrectNodeParent, incorrectNodeDesc,
      ARRAY_SIZE(incorrectNodeDesc));
  ck_assert_uint_ne(res, E_OK);

  struct FsNode * const txtNode =
      fsOpenNode(handle, PATH_HOME_USER_FILE);
  ck_assert_ptr_nonnull(txtNode);
  mallocHookFails = 1;
  res = fsNodeWrite(txtNode, FS_NODE_DATA, 0, data, MAX_BUFFER_LENGTH, 0);
  ck_assert_uint_ne(res, E_OK);
  fsNodeFree(txtNode);

  fsNodeFree(incorrectNodeParent);
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
  FsSpace used;

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

  FsSpace used;

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
