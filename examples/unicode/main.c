/*
 * main.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <bits.h>
#include <unicode.h>
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif
/*----------------------------------------------------------------------------*/
#define MAX_BUFFER_LENGTH 64
/*----------------------------------------------------------------------------*/
static void dumpChar16String(const char16_t *array, unsigned int length)
{
  DEBUG_PRINT("UTF-16: ");
  for (unsigned int index = 0; index < length; ++index)
    DEBUG_PRINT("%04X ", (unsigned int)array[index] & 0xFFFF);
  DEBUG_PRINT("\n");
}
/*----------------------------------------------------------------------------*/
static void dumpChar8String(const char *array, unsigned int length)
{
  DEBUG_PRINT("UTF-8:  ");
  for (unsigned int index = 0; index < length; ++index)
    DEBUG_PRINT("%02X ", (unsigned int)array[index] & 0xFF);
  DEBUG_PRINT("\n");
}
/*----------------------------------------------------------------------------*/
static void performStringComparison(const char *input, unsigned int inputLength,
    const char16_t *sample, unsigned int sampleLength)
{
  char16_t frontBuffer[MAX_BUFFER_LENGTH];
  char backBuffer[MAX_BUFFER_LENGTH];
  uint16_t count;

#ifdef NDEBUG
  /* Suppress warnings */
  (void)inputLength;
  (void)sample;
  (void)sampleLength;
#endif

  count = uToUtf16(frontBuffer, input, sizeof(frontBuffer));
  assert(count == sampleLength);
  for (unsigned int index = 0; index <= count; ++index)
  {
    assert(frontBuffer[index] == sample[index]);
  }
  dumpChar16String(frontBuffer, count);

  count = uFromUtf16(backBuffer, frontBuffer, sizeof(backBuffer));
  assert(count == inputLength);
  for (unsigned int index = 0; index <= count; ++index)
  {
    assert(backBuffer[index] == input[index]);
  }
  dumpChar8String(backBuffer, count);
}
/*----------------------------------------------------------------------------*/
static void performUtf8ToUtf16(void)
{
  const char basePlaneTest[] = "String";
  const char16_t basePlaneSample[] = {
      0x0053, 0x0074, 0x0072, 0x0069, 0x006E, 0x0067, 0x0000
  };

  DEBUG_PRINT("Characters from U+0000 to U+007F\n");
  performStringComparison(basePlaneTest, strlen(basePlaneTest),
      basePlaneSample, ARRAY_SIZE(basePlaneSample) - 1);

  const char twoByteTest[] = "\xD0\xA1\xD1\x82\xD1\x80\xD0\xBE\xD0\xBA\xD0\xB0";
  const char16_t twoByteSample[] = {
      0x0421, 0x0442, 0x0440, 0x043E, 0x043A, 0x0430, 0x0000
  };

  DEBUG_PRINT("Characters from U+0080 to U+07FF\n");
  performStringComparison(twoByteTest, strlen(twoByteTest),
      twoByteSample, ARRAY_SIZE(twoByteSample) - 1);

  const char threeByteTest[] =
      "\xE4\xBF\xA1\xE6\x81\xAF\xE5\x8C\x96\xE7\x9A\x84"
      "\xE7\xA4\xBE\xE4\xBC\x9A\xE5\xBD\xB1\xE5\x93\x8D";
  const char16_t threeByteSample[] = {
      0x4FE1, 0x606F, 0x5316, 0x7684, 0x793E, 0x4F1A, 0x5F71, 0x54CD, 0x0000
  };

  DEBUG_PRINT("Characters from U+0800 to U+FFFF\n");
  performStringComparison(threeByteTest, strlen(threeByteTest),
      threeByteSample, ARRAY_SIZE(threeByteSample) - 1);

  const char fourByteTest[] =
      "\xF0\x90\x8C\x80\xF0\x90\x8C\x81\xF0\x90\x8C\x82"
      "\xF0\x90\x8C\x83\xF0\x90\x8C\x84\xF0\x90\x8C\x85";
  const char16_t fourByteSample[] = {
      0xD800, 0xDF00, 0xD800, 0xDF01, 0xD800, 0xDF02,
      0xD800, 0xDF03, 0xD800, 0xDF04, 0xD800, 0xDF05,
      0x0000
  };

  DEBUG_PRINT("Characters from U+10000 to U+1FFFF\n");
  performStringComparison(fourByteTest, strlen(fourByteTest),
      fourByteSample, ARRAY_SIZE(fourByteSample) - 1);
}
/*----------------------------------------------------------------------------*/
int main(void)
{
  /* TODO Test overflows */
  performUtf8ToUtf16();

  printf("Passed\n");

  return 0;
}
