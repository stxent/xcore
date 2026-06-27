/*
 * main.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/crc/crc7_mmc.h>
#include <xcore/crc/crc8_maxim.h>
#include <xcore/crc/crc16_ccitt.h>
#include <xcore/crc/crc32_ieee.h>
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_DEBUG
#  define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#  define DEBUG_PRINT(...) do {} while (0)
#endif
/*----------------------------------------------------------------------------*/
static const char payload[] = "Pack my box with five dozen liquor jugs";
static const size_t payloadLengthFull = sizeof(payload) - 1;
static const size_t payloadLengthPart = (sizeof(payload) - 1) / 2;
static const size_t payloadLengthLeft = payloadLengthFull - payloadLengthPart;
/*----------------------------------------------------------------------------*/
START_TEST(testCrc7MMCAlgorithm)
{
  static const uint8_t initial = 0;
  static const uint8_t nominal = 0x23;
  uint8_t result;

  result = crc7MMCUpdate(initial, "", 0);
  DEBUG_PRINT("CRC-7 of empty string: 0x%02X\r\n", result);
  ck_assert(result == initial);

  result = crc7MMCUpdate(initial, payload, payloadLengthFull);
  DEBUG_PRINT("CRC-7 of data string: 0x%02X\r\n", result);
  ck_assert(result == nominal);

  result = crc7MMCUpdate(initial, payload, payloadLengthPart);
  result = crc7MMCUpdate(result, payload + payloadLengthPart,
      payloadLengthLeft);
  DEBUG_PRINT("CRC-7 of composite string: 0x%02X\r\n", result);
  ck_assert(result == nominal);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testCrc8MaximAlgorithm)
{
  static const uint8_t initial = 0;
  static const uint8_t nominal = 0xE9;
  uint8_t result;

  result = crc8MaximUpdate(initial, "", 0);
  DEBUG_PRINT("CRC-8-Maxim of empty string: 0x%02X\r\n", result);
  ck_assert(result == initial);

  result = crc8MaximUpdate(initial, payload, payloadLengthFull);
  DEBUG_PRINT("CRC-8-Maxim of data string: 0x%02X\r\n", result);
  ck_assert(result == nominal);

  result = crc8MaximUpdate(initial, payload, payloadLengthPart);
  result = crc8MaximUpdate(result, payload + payloadLengthPart,
      payloadLengthLeft);
  DEBUG_PRINT("CRC-8-Maxim of composite string: 0x%02X\r\n", result);
  ck_assert(result == nominal);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testCrc16CCITTAlgorithm)
{
  static const uint16_t initial = 0xFFFF;
  static const uint16_t nominal = 0xB035;
  uint16_t result;

  result = crc16CCITTUpdate(initial, "", 0);
  DEBUG_PRINT("CRC-16-CCITT of empty string: 0x%04X\r\n", result);
  ck_assert(result == initial);

  result = crc16CCITTUpdate(initial, payload, payloadLengthFull);
  DEBUG_PRINT("CRC-16-CCITT of data string: 0x%04X\r\n", result);
  ck_assert(result == nominal);

  result = crc16CCITTUpdate(initial, payload, payloadLengthPart);
  result = crc16CCITTUpdate(result, payload + payloadLengthPart,
      payloadLengthLeft);
  DEBUG_PRINT("CRC-16-CCITT of composite string: 0x%04X\r\n", result);
  ck_assert(result == nominal);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testCrc32IEEEAlgorithm)
{
  static const uint32_t initial = 0;
  static const uint32_t nominal = 0xE4500145UL;
  uint32_t result;

  result = crc32IEEEUpdate(initial, "", 0);
  DEBUG_PRINT("CRC-32-IEEE of empty string: 0x%08X\r\n", result);
  ck_assert(result == initial);

  result = crc32IEEEUpdate(initial, payload, payloadLengthFull);
  DEBUG_PRINT("CRC-32-IEEE of data string: 0x%08X\r\n", result);
  ck_assert(result == nominal);

  result = crc32IEEEUpdate(initial, payload, payloadLengthPart);
  result = crc32IEEEUpdate(result, payload + payloadLengthPart,
      payloadLengthLeft);
  DEBUG_PRINT("CRC-32-IEEE of composite string: 0x%08X\r\n", result);
  ck_assert(result == nominal);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Checksums");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testCrc7MMCAlgorithm);
  tcase_add_test(testcase, testCrc8MaximAlgorithm);
  tcase_add_test(testcase, testCrc16CCITTAlgorithm);
  tcase_add_test(testcase, testCrc32IEEEAlgorithm);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
