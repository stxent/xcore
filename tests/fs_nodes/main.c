/*
 * main.c
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include "tfs.h"
#include <xcore/fs/utils.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#define MAX_BUFFER_LENGTH 128
/*----------------------------------------------------------------------------*/
static void freeNode(struct FsHandle *, const char *);
static void makeNode(struct FsHandle *, const char *);
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
static void freeTestHandle(struct FsHandle *handle)
{
  /* Level 3 inside "/home/user directory" */
  freeNode(handle, "/home/user directory/some file.txt");
  freeNode(handle, "/home/user directory/one more file.bmp");

  /* Level 2 inside "/home" */
  freeNode(handle, "/home/root directory");
  freeNode(handle, "/home/user directory");

  /* Level 1 inside "/" */
  freeNode(handle, "/sys");
  freeNode(handle, "/home");
  freeNode(handle, "/boot");

  deinit(handle);
}
/*----------------------------------------------------------------------------*/
static struct FsHandle *makeTestHandle()
{
  struct FsHandle * const handle = init(TfsHandle, 0);

  /* Level 1 inside "/" */
  makeNode(handle, "/boot");
  makeNode(handle, "/home");
  makeNode(handle, "/sys");

  /* Level 2 inside "/home" */
  makeNode(handle, "/home/root directory");
  makeNode(handle, "/home/user directory");

  /* Level 3 inside "/home/user directory" */
  makeNode(handle, "/home/user directory/some file.txt");
  makeNode(handle, "/home/user directory/one more file.bmp");

  return handle;
}
/*----------------------------------------------------------------------------*/
static void freeNode(struct FsHandle *handle, const char *path)
{
  struct FsNode * const root = fsOpenBaseNode(handle, path);
  ck_assert_ptr_nonnull(root);

  struct FsNode * const node = fsOpenNode(handle, path);
  ck_assert_ptr_nonnull(node);
  const enum Result res = fsNodeRemove(root, node);
  ck_assert(res == E_OK);

  fsNodeFree(node);
  fsNodeFree(root);
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
      },  {
          &access,
          sizeof(access),
          FS_NODE_ACCESS
      }
  };
  const enum Result res = fsNodeCreate(parent, desc, ARRAY_SIZE(desc));
  ck_assert(res == E_OK);
  fsNodeFree(parent);

  struct FsNode *node = fsOpenNode(handle, path);
  ck_assert_ptr_nonnull(node);
  fsNodeFree(node);
}
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

  struct FsNode * const dirNode =
      fsOpenBaseNode(handle, "/home/user directory/some file.txt");
  ck_assert_ptr_nonnull(dirNode);
  struct FsNode * const txtNode =
      fsOpenNode(handle, "/home/user directory/some file.txt");
  ck_assert_ptr_nonnull(txtNode);
  ck_assert_ptr_ne(dirNode, txtNode);

  fsNodeFree(dirNode);
  fsNodeFree(txtNode);

  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testTfsErrors)
{
  struct FsHandle * const handle = makeTestHandle();

  /* Try to open a child of empty node */
  struct FsNode * const emptyNode =
      fsOpenNode(handle, "/home/user directory/some file.txt");
  ck_assert_ptr_nonnull(emptyNode);
  struct FsNode * const emptyNodeChild = fsNodeHead(emptyNode);
  ck_assert_ptr_null(emptyNodeChild);
  fsNodeFree(emptyNode);

  /* Try to remove nondescendant node */
  struct FsNode * const nondescNodeA = fsOpenNode(handle, "/home");
  ck_assert_ptr_nonnull(nondescNodeA);
  struct FsNode * const nondescNodeB = fsOpenNode(handle, "/boot");
  ck_assert_ptr_nonnull(nondescNodeB);
  const enum Result nondescRes = fsNodeRemove(nondescNodeA, nondescNodeB);
  ck_assert(nondescRes == E_ENTRY);
  fsNodeFree(nondescNodeA);
  fsNodeFree(nondescNodeB);

  /* Try to read unavailable atttributes */
  struct FsNode * const attrNode = fsOpenNode(handle, "/home");
  ck_assert_ptr_nonnull(attrNode);
  char buffer[MAX_BUFFER_LENGTH];
  size_t count;

  const enum Result attrNodeNameResA =
      fsNodeRead(attrNode, FS_NODE_NAME, 0, buffer, sizeof(buffer), &count);
  ck_assert(attrNodeNameResA == E_OK);
  ck_assert_str_eq(buffer, "home");
  ck_assert_uint_eq(count, strlen("home") + 1);
  const enum Result attrNodeNameResB =
      fsNodeRead(attrNode, FS_NODE_NAME, 0, buffer, sizeof(buffer), 0);
  ck_assert(attrNodeNameResB == E_OK);
  ck_assert_str_eq(buffer, "home");
  const enum Result attrNodeNameResC =
      fsNodeRead(attrNode, FS_NODE_NAME, 1, buffer, sizeof(buffer), 0);
  ck_assert(attrNodeNameResC != E_OK);
  const enum Result attrNodeNameResD =
      fsNodeRead(attrNode, FS_NODE_NAME, 0, buffer, 1, 0);
  ck_assert(attrNodeNameResD != E_OK);
  const enum Result attrNodeNameResE =
      fsNodeRead(attrNode, FS_NODE_ACCESS, 0, buffer, sizeof(buffer), 0);
  ck_assert(attrNodeNameResE != E_OK);

  fsNodeFree(attrNode);

  /* Iterate over handle head */
  struct FsNode * const headNode = fsOpenNode(handle, "/");
  ck_assert_ptr_nonnull(headNode);
  const enum Result headNodeRes = fsNodeNext(headNode);
  ck_assert(headNodeRes == E_ENTRY);
  fsNodeFree(headNode);

  /* Iterate to the end of directory */
  struct FsNode * const endNode = fsOpenNode(handle, "/");
  ck_assert_ptr_nonnull(headNode);
  struct FsNode * const endNodeChild = fsNodeHead(endNode);
  ck_assert_ptr_nonnull(endNodeChild);
  const enum Result endNode2Res = fsNodeNext(endNodeChild);
  ck_assert(endNode2Res == E_OK);
  const enum Result endNode3Res = fsNodeNext(endNodeChild);
  ck_assert(endNode3Res == E_OK);
  const enum Result endNode4Res = fsNodeNext(endNodeChild);
  ck_assert(endNode4Res == E_ENTRY);
  fsNodeFree(endNode);
  fsNodeFree(endNodeChild);

  /* Sync handle */
  fsHandleSync(handle);

  /* Create incorrect nodes */
  struct FsNode * const incorrectNodeParent = fsOpenNode(handle, "/home");
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
  ck_assert(incorrectNodeResA != E_OK);

  const enum Result incorrectNodeResB = fsNodeCreate(incorrectNodeParent,
      incorrectNodeDesc, ARRAY_SIZE(incorrectNodeDesc));
  ck_assert(incorrectNodeResB != E_OK);

  fsNodeFree(incorrectNodeParent);

  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testTfsMemoryErrors)
{
  struct FsHandle * const handle = makeTestHandle();
  struct FsNode * const incorrectNodeParent = fsOpenNode(handle, "/home");
  ck_assert_ptr_nonnull(incorrectNodeParent);

  const struct FsFieldDescriptor incorrectNodeDesc[] = {
      {
          "error",
          strlen("error") + 1,
          FS_NODE_NAME
      }
  };

  mallocHookFails = 1;
  const enum Result incorrectNodeResA = fsNodeCreate(incorrectNodeParent,
      incorrectNodeDesc, 0);
  ck_assert(incorrectNodeResA != E_OK);

  mallocHookFails = 2;
  const enum Result incorrectNodeResB = fsNodeCreate(incorrectNodeParent,
      incorrectNodeDesc, ARRAY_SIZE(incorrectNodeDesc));
  ck_assert(incorrectNodeResB != E_OK);

  mallocHookFails = 3;
  const enum Result incorrectNodeResC = fsNodeCreate(incorrectNodeParent,
      incorrectNodeDesc, ARRAY_SIZE(incorrectNodeDesc));
  ck_assert(incorrectNodeResC != E_OK);

  fsNodeFree(incorrectNodeParent);
  freeTestHandle(handle);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("TestFileSystem");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testPartExtraction);
  tcase_add_test(testcase, testPathFollowing);
  tcase_add_test(testcase, testTfsErrors);
  tcase_add_test(testcase, testTfsMemoryErrors);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
