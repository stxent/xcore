/*
 * main.c
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <xcore/fs/utils.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#define MAX_BUFFER_LENGTH 128
/*----------------------------------------------------------------------------*/
START_TEST(testBaseNameExtraction)
{
  static const char empty[MAX_BUFFER_LENGTH] = {0};
  char buffer[MAX_BUFFER_LENGTH];

  static const char path0Src[] = "/home/user directory/some file.txt";
  static const char path0Dst[] = "/home/user directory";
  const bool path0Res = fsExtractBaseName(buffer, path0Src);
  ck_assert(path0Res == true);
  ck_assert_str_eq(buffer, path0Dst);

  memset(buffer, 0, MAX_BUFFER_LENGTH);
  static const char path1Src[] = "/ file with spaces .txt";
  const bool path1Res = fsExtractBaseName(buffer, path1Src);
  ck_assert_mem_eq(buffer, empty, MAX_BUFFER_LENGTH);
  ck_assert(path1Res == false);

  static const char path2Src[] = "/\xD0\xBF\xD0\xB0\xD0\xBF\xD0\xBA\xD0\xB0/";
  static const char path2Dst[] = "/\xD0\xBF\xD0\xB0\xD0\xBF\xD0\xBA\xD0\xB0";
  const bool path2Res = fsExtractBaseName(buffer, path2Src);
  ck_assert(path2Res == true);
  ck_assert_str_eq(buffer, path2Dst);

  memset(buffer, 0, MAX_BUFFER_LENGTH);
  const bool path3Res = fsExtractBaseName(buffer, "");
  ck_assert_mem_eq(buffer, empty, MAX_BUFFER_LENGTH);
  ck_assert(path3Res == false);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testChunkExtraction)
{
  char buffer[MAX_BUFFER_LENGTH];

  static const char path0Src[] = "/home/user directory/some file.txt";
  static const char path0Step0[] = "/";
  static const char path0Step1[] = "home";
  static const char path0Step2[] = "user directory";
  static const char path0Step3[] = "some file.txt";
  static const char path0Step4[] = "";

  const char *path0Res0 = fsGetChunk(buffer, path0Src);
  ck_assert_str_eq(buffer, path0Step0);
  ck_assert_ptr_eq(path0Res0, path0Src + strlen(path0Step0));

  const char *path0Res1 = fsGetChunk(buffer, path0Res0);
  ck_assert_str_eq(buffer, path0Step1);

  const char *path0Res2 = fsGetChunk(buffer, path0Res1);
  ck_assert_str_eq(buffer, path0Step2);

  const char *path0Res3 = fsGetChunk(buffer, path0Res2);
  ck_assert_str_eq(buffer, path0Step3);

  fsGetChunk(buffer, path0Res3);
  ck_assert_str_eq(buffer, path0Step4);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testChunkStripping)
{
  char buffer[MAX_BUFFER_LENGTH];

  static const char path0Src[] = "//home//user directory";
  static const char path0Step0[] = "/";
  static const char path0Step1[] = "home";
  static const char path0Step2[] = "user directory";

  const char *path0Res0 = fsGetChunk(buffer, path0Src);
  ck_assert_str_eq(buffer, path0Step0);

  const char *path0Res1 = fsGetChunk(buffer, path0Res0);
  ck_assert_str_eq(buffer, path0Step1);

  fsGetChunk(buffer, path0Res1);
  ck_assert_str_eq(buffer, path0Step2);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testNameExtraction)
{
  static const char path0Src[] = "/home/user directory/some file.txt";
  static const char path0Dst[] = "some file.txt";
  const char *path0Res = fsExtractName(path0Src);
  ck_assert_str_eq(path0Res, path0Dst);

  static const char path1Src[] = "/ file with spaces .txt";
  static const char path1Dst[] = " file with spaces .txt";
  const char *path1Res = fsExtractName(path1Src);
  ck_assert_str_eq(path1Res, path1Dst);

  static const char path2Src[] = "\xD1\x84\xD0\xB0\xD0\xB9\xD0\xBB.txt";
  const char *path2Res = fsExtractName(path2Src);
  ck_assert_str_eq(path2Res, path2Src);

  static const char path3Src[] = "/home/user/";
  const char *path3Res = fsExtractName(path3Src);
  ck_assert_ptr_null(path3Res);

  const char *path4Res = fsExtractName("");
  ck_assert_ptr_null(path4Res);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testNameStripping)
{
  char path0Src[] = "/home/user directory/some file.txt";
  static const char path0Dst[] = "/home/user directory";
  const bool path0Res = fsStripName(path0Src);
  ck_assert(path0Res == true);
  ck_assert_str_eq(path0Src, path0Dst);

  char path1Src[] = "some file.txt";
  const bool path1Res = fsStripName(path1Src);
  ck_assert(path1Res == true);
  ck_assert_str_eq(path1Src, "");

  const bool path2Res = fsStripName("/");
  ck_assert(path2Res == false);

  const bool path3Res = fsStripName("");
  ck_assert(path3Res == false);
}
END_TEST
/*----------------------------------------------------------------------------*/
void joinPathsAndCompare(const char *a, const char *b, const char *ref)
{
  char buffer[MAX_BUFFER_LENGTH];
  fsJoinPaths(buffer, a, b);
  ck_assert_str_eq(buffer, ref);
}
/*----------------------------------------------------------------------------*/
START_TEST(testPathJoining)
{
  joinPathsAndCompare(
      "/home/",
      "file.txt",
      "/home/file.txt");

  joinPathsAndCompare(
      "/home",
      "some file.txt",
      "/home/some file.txt");

  joinPathsAndCompare(
      "/home/",
      "",
      "/home");

  joinPathsAndCompare(
      "/home",
      "",
      "/home");

  joinPathsAndCompare(
      "home",
      "",
      "/home");

  joinPathsAndCompare(
      "/",
      "file.txt",
      "/file.txt");

  joinPathsAndCompare(
      "/a",
      "file.txt",
      "/a/file.txt");

  joinPathsAndCompare(
      "",
      "file.txt",
      "/file.txt");

  joinPathsAndCompare(
      "",
      "",
      "/");
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("FileSystemUtils");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testBaseNameExtraction);
  tcase_add_test(testcase, testChunkExtraction);
  tcase_add_test(testcase, testChunkStripping);
  tcase_add_test(testcase, testNameExtraction);
  tcase_add_test(testcase, testNameStripping);
  tcase_add_test(testcase, testPathJoining);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
