/*
 * main.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <xcore/crc/crc7.h>
#include <xcore/crc/crc8_dallas.h>
#include <xcore/crc/crc16_ccitt.h>
#include <xcore/crc/crc32.h>
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif
/*----------------------------------------------------------------------------*/
static const char *emptyString = "";
static const char *dataString = "Informatization";
/*----------------------------------------------------------------------------*/
static void performCrc7Test(void)
{
  const char * const name = "CRC-7";
  const uint8_t initial = 0;
  uint8_t result;

  DEBUG_PRINT("Test %s\n", name);

  result = crc7Update(initial, emptyString, strlen(emptyString));
  DEBUG_PRINT("%s of empty string: 0x%02X\n", name, result);
  assert(result == 0x00);

  result = crc7Update(initial, dataString, strlen(dataString));
  DEBUG_PRINT("%s of data string: 0x%02X\n", name, result);
  assert(result == 0x0F);

  (void)name;
  (void)result;
}
/*----------------------------------------------------------------------------*/
static void performCrc8DallasTest(void)
{
  const char * const name = "CRC-8-Dallas";
  const uint8_t initial = 0;
  uint8_t result;

  DEBUG_PRINT("Test %s\n", name);

  result = crc8DallasUpdate(initial, emptyString, strlen(emptyString));
  DEBUG_PRINT("%s of empty string: 0x%02X\n", name, result);
  assert(result == 0x00);

  result = crc8DallasUpdate(initial, dataString, strlen(dataString));
  DEBUG_PRINT("%s of data string: 0x%02X\n", name, result);
  assert(result == 0x3B);

  (void)name;
  (void)result;
}
/*----------------------------------------------------------------------------*/
static void performCrc16CCITTTest(void)
{
  const char * const name = "CRC-16-CCITT";
  const uint16_t initial = 0xFFFF;
  uint16_t result;

  DEBUG_PRINT("Test %s\n", name);

  result = crc16CCITTUpdate(initial, emptyString, strlen(emptyString));
  DEBUG_PRINT("%s of empty string: 0x%04X\n", name, result);
  assert(result == 0xFFFF);

  result = crc16CCITTUpdate(initial, dataString, strlen(dataString));
  DEBUG_PRINT("%s of data string: 0x%04X\n", name, result);
  assert(result == 0x6472);

  (void)name;
  (void)result;
}
/*----------------------------------------------------------------------------*/
static void performCrc32Test(void)
{
  const char * const name = "CRC-32";
  const uint32_t initial = 0;
  uint32_t result;

  DEBUG_PRINT("Test %s\n", name);

  result = crc32Update(initial, emptyString, strlen(emptyString));
  DEBUG_PRINT("%s of empty string: 0x%08X\n", name, result);
  assert(result == 0x00000000);

  result = crc32Update(initial, dataString, strlen(dataString));
  DEBUG_PRINT("%s of data string: 0x%08X\n", name, result);
  assert(result == 0xC268A8E6);

  (void)name;
  (void)result;
}
/*----------------------------------------------------------------------------*/
int main(void)
{
  performCrc7Test();
  performCrc8DallasTest();
  performCrc16CCITTTest();
  performCrc32Test();

  printf("Passed\n");

  return 0;
}
