/*
 * main.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/crc/crc7.h>
#include <xcore/crc/crc8_dallas.h>
#include <xcore/crc/crc16_ccitt.h>
#include <xcore/crc/crc32.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif
/*----------------------------------------------------------------------------*/
static const char payload[] = "Informatization";
/*----------------------------------------------------------------------------*/
START_TEST(testCrc7Algorithm)
{
  const uint8_t initial = 0;
  uint8_t result;

  result = crc7Update(initial, "", 0);
  DEBUG_PRINT("CRC-7 of empty string: 0x%02X\n", result);
  ck_assert(result == 0x00);

  result = crc7Update(initial, payload, strlen(payload));
  DEBUG_PRINT("CRC-7 of data string: 0x%02X\n", result);
  ck_assert(result == 0x0F);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testCrc8DallasAlgorithm)
{
  const uint8_t initial = 0;
  uint8_t result;

  result = crc8DallasUpdate(initial, "", 0);
  DEBUG_PRINT("CRC-8-Dallas of empty string: 0x%02X\n", result);
  ck_assert(result == 0x00);

  result = crc8DallasUpdate(initial, payload, strlen(payload));
  DEBUG_PRINT("CRC-8-Dallas of data string: 0x%02X\n", result);
  ck_assert(result == 0x3B);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testCrc16CCITTAlgorithm)
{
  const uint16_t initial = 0xFFFF;
  uint16_t result;

  result = crc16CCITTUpdate(initial, "", 0);
  DEBUG_PRINT("CRC-16-CCITT of empty string: 0x%04X\n", result);
  ck_assert(result == 0xFFFF);

  result = crc16CCITTUpdate(initial, payload, strlen(payload));
  DEBUG_PRINT("CRC-16-CCITT of data string: 0x%04X\n", result);
  ck_assert(result == 0x6472);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testCrc32Algorithm)
{
  const uint32_t initial = 0;
  uint32_t result;

  result = crc32Update(initial, "", 0);
  DEBUG_PRINT("CRC-32 of empty string: 0x%08X\n", result);
  ck_assert(result == 0x00000000);

  result = crc32Update(initial, payload, strlen(payload));
  DEBUG_PRINT("CRC-32 of data string: 0x%08X\n", result);
  ck_assert(result == 0xC268A8E6);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Checksums");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testCrc7Algorithm);
  tcase_add_test(testcase, testCrc8DallasAlgorithm);
  tcase_add_test(testcase, testCrc16CCITTAlgorithm);
  tcase_add_test(testcase, testCrc32Algorithm);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
