/*
 * main.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <check.h>
#include <stdlib.h>
#include <xcore/unicode.h>
/*----------------------------------------------------------------------------*/
#define MAX_BUFFER_LENGTH 64

#ifdef CONFIG_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif
/*----------------------------------------------------------------------------*/
static void dumpChar16String(const char16_t *array, size_t length)
{
#ifndef CONFIG_DEBUG
  /* Suppress warnings */
  (void)array;
#endif

  DEBUG_PRINT("UTF-16: ");
  for (size_t index = 0; index < length; ++index)
    DEBUG_PRINT("%04X ", (size_t)array[index] & 0xFFFF);
  DEBUG_PRINT("\n");
}
/*----------------------------------------------------------------------------*/
static void dumpChar8String(const char *array, size_t length)
{
#ifndef CONFIG_DEBUG
  /* Suppress warnings */
  (void)array;
#endif

  DEBUG_PRINT("UTF-8:  ");
  for (size_t index = 0; index < length; ++index)
    DEBUG_PRINT("%02X ", (size_t)array[index] & 0xFF);
  DEBUG_PRINT("\n");
}
/*----------------------------------------------------------------------------*/
static void compareStrings(const char *input, size_t inputLength,
    const char16_t *sample, size_t sampleLength)
{
  char16_t frontBuffer[MAX_BUFFER_LENGTH];
  char backBuffer[MAX_BUFFER_LENGTH];
  uint16_t count;

  /* Check conversion from UTF-8 to UTF-16 */
  count = uToUtf16(frontBuffer, input, sizeof(frontBuffer));
  ck_assert_uint_eq(count, sampleLength);
  for (size_t index = 0; index <= count; ++index)
  {
    ck_assert(frontBuffer[index] == sample[index]);
  }
  dumpChar16String(frontBuffer, count);

  /* Check length estimation of the resulting UTF-16 string */
  count = uLengthToUtf16(input);
  ck_assert_uint_eq(count, sampleLength);

  /* Check conversion from UTF-16 to UTF-8 */
  count = uFromUtf16(backBuffer, frontBuffer, sizeof(backBuffer));
  ck_assert_uint_eq(count, inputLength);
  for (size_t index = 0; index <= count; ++index)
  {
    ck_assert(backBuffer[index] == input[index]);
  }
  dumpChar8String(backBuffer, count);

  /* Check length estimation of the resulting UTF-8 string */
  count = uLengthFromUtf16(frontBuffer);
  ck_assert_uint_eq(count, inputLength);
}
/*----------------------------------------------------------------------------*/
START_TEST(testErrorCases)
{
  /* Small output buffer test */
  static const char16_t shortUtf16Src[] = {
      0x0442, 0x0435, 0x0441, 0x0442, 0x0000
  };
  static const char shortUtf16Dst[] = "\xD1\x82\xD0\xB5\xD1\x81";
  char shortUtf16Buffer[sizeof(shortUtf16Src) - 2];

  const size_t shortUtf16Res = uFromUtf16(shortUtf16Buffer,
      shortUtf16Src, sizeof(shortUtf16Buffer));
  ck_assert_uint_eq(shortUtf16Res, strlen(shortUtf16Dst));
  ck_assert_str_eq(shortUtf16Buffer, shortUtf16Dst);

  /* Dropped surrogate pairs */
  static const char16_t surrogateUtf16Src[] = {
      0xD845, 0xDC84, 0xDF19, 0xD846, 0xD846, 0xDFA6, 0xDE4F, 0xD83D, 0x0000
  };
  static const char surrogateUtf16Dst[] = "\xF0\xA1\x92\x84\xF0\xA1\xAE\xA6";
  char surrogateUtf16Buffer[MAX_BUFFER_LENGTH];

  const size_t surrogateUtf16Len = uLengthFromUtf16(surrogateUtf16Src);
  ck_assert_uint_eq(surrogateUtf16Len, strlen(surrogateUtf16Dst));
  const size_t surrogateUtf16Res = uFromUtf16(surrogateUtf16Buffer,
      surrogateUtf16Src, sizeof(surrogateUtf16Buffer));
  ck_assert_uint_eq(surrogateUtf16Res, strlen(surrogateUtf16Dst));
  ck_assert_str_eq(surrogateUtf16Buffer, surrogateUtf16Dst);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testUtf8ToUtf16Conversion)
{
  static const char basePlaneTest[] = "String";
  static const char16_t basePlaneSample[] = {
      0x0053, 0x0074, 0x0072, 0x0069, 0x006E, 0x0067, 0x0000
  };

  DEBUG_PRINT("Characters from U+0000 to U+007F\n");
  compareStrings(basePlaneTest, strlen(basePlaneTest),
      basePlaneSample, ARRAY_SIZE(basePlaneSample) - 1);

  static const char twoByteTest[] =
      "\xD0\xA1\xD1\x82\xD1\x80\xD0\xBE\xD0\xBA\xD0\xB0";
  static const char16_t twoByteSample[] = {
      0x0421, 0x0442, 0x0440, 0x043E, 0x043A, 0x0430, 0x0000
  };

  DEBUG_PRINT("Characters from U+0080 to U+07FF\n");
  compareStrings(twoByteTest, strlen(twoByteTest),
      twoByteSample, ARRAY_SIZE(twoByteSample) - 1);

  static const char threeByteTest[] =
      "\xE4\xBF\xA1\xE6\x81\xAF\xE5\x8C\x96\xE7\x9A\x84"
      "\xE7\xA4\xBE\xE4\xBC\x9A\xE5\xBD\xB1\xE5\x93\x8D";
  static const char16_t threeByteSample[] = {
      0x4FE1, 0x606F, 0x5316, 0x7684, 0x793E, 0x4F1A, 0x5F71, 0x54CD, 0x0000
  };

  DEBUG_PRINT("Characters from U+0800 to U+FFFF\n");
  compareStrings(threeByteTest, strlen(threeByteTest),
      threeByteSample, ARRAY_SIZE(threeByteSample) - 1);

  static const char fourByteTest[] =
      "\xF0\x90\x8C\x80\xF0\x90\x8C\x81\xF0\x90\x8C\x82"
      "\xF0\x90\x8C\x83\xF0\x90\x8C\x84\xF0\x90\x8C\x85";
  static const char16_t fourByteSample[] = {
      0xD800, 0xDF00, 0xD800, 0xDF01, 0xD800, 0xDF02,
      0xD800, 0xDF03, 0xD800, 0xDF04, 0xD800, 0xDF05,
      0x0000
  };

  DEBUG_PRINT("Characters from U+10000 to U+1FFFF\n");
  compareStrings(fourByteTest, strlen(fourByteTest),
      fourByteSample, ARRAY_SIZE(fourByteSample) - 1);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Unicode");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testErrorCases);
  tcase_add_test(testcase, testUtf8ToUtf16Conversion);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
