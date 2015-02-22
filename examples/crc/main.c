/*
 * main.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <crc.h>
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_CRC_DEBUG
#include <stdio.h>
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif
/*----------------------------------------------------------------------------*/
static const char *emptyString = "";
static const char *dataString = "Informatization";
/*----------------------------------------------------------------------------*/
const struct CrcEngineClass * const Crc7;
const struct CrcEngineClass * const Crc8Dallas;
const struct CrcEngineClass * const Crc16;
/*----------------------------------------------------------------------------*/
static void performCrc7Test(void)
{
  const uint32_t initial = 0;
  struct CrcEngine *crc7;

#ifndef NDEBUG
  uint32_t result;
#else
  uint32_t result __attribute__((unused));
#endif

  DEBUG_PRINT("Test CRC-7\n");

  crc7 = init(Crc7, 0);
  assert(crc7 != 0);

  result = crcUpdate(crc7, initial, (const uint8_t *)emptyString,
      strlen(emptyString));
  DEBUG_PRINT("CRC-7 of empty string: 0x%02X\n", result);
  assert(result == 0x00);

  result = crcUpdate(crc7, initial, (const uint8_t *)dataString,
      strlen(dataString));
  DEBUG_PRINT("CRC-7 of data string: 0x%02X\n", result);
  assert(result == 0x0F);

  deinit(crc7);
}
/*----------------------------------------------------------------------------*/
static void performCrc8DallasTest(void)
{
  const uint32_t initial = 0;
  struct CrcEngine *crc8Dallas;

#ifndef NDEBUG
  uint32_t result;
#else
  uint32_t result __attribute__((unused));
#endif

  DEBUG_PRINT("Test CRC-8-Dallas\n");

  crc8Dallas = init(Crc8Dallas, 0);
  assert(crc8Dallas != 0);

  result = crcUpdate(crc8Dallas, initial, (const uint8_t *)emptyString,
      strlen(emptyString));
  DEBUG_PRINT("CRC-8-Dallas of empty string: 0x%02X\n", result);
  assert(result == 0x00);

  result = crcUpdate(crc8Dallas, initial, (const uint8_t *)dataString,
      strlen(dataString));
  DEBUG_PRINT("CRC-8-Dallas of data string: 0x%02X\n", result);
  assert(result == 0x3B);

  deinit(crc8Dallas);
}
/*----------------------------------------------------------------------------*/
static void performCrc16Test(void)
{
  const uint32_t initial = 0xFFFF;
  struct CrcEngine *crc16;

#ifndef NDEBUG
  uint32_t result;
#else
  uint32_t result __attribute__((unused));
#endif

  DEBUG_PRINT("Test CRC-16\n");

  crc16 = init(Crc16, 0);
  assert(crc16 != 0);

  result = crcUpdate(crc16, initial, (const uint8_t *)emptyString,
      strlen(emptyString));
  DEBUG_PRINT("CRC-16 of empty string: 0x%04X\n", result);
  assert(result == 0xFFFF);

  result = crcUpdate(crc16, initial, (const uint8_t *)dataString,
      strlen(dataString));
  DEBUG_PRINT("CRC-16 of data string: 0x%04X\n", result);
  assert(result == 0x6472);

  deinit(crc16);
}
/*----------------------------------------------------------------------------*/
int main(void)
{
  DEBUG_PRINT("CRC-7 module location: 0x%08lX\n", (unsigned long)Crc7);
  DEBUG_PRINT("CRC-8-Dallas module location: 0x%08lX\n",
      (unsigned long)Crc8Dallas);
  DEBUG_PRINT("CRC-16 module location: 0x%08lX\n", (unsigned long)Crc16);

  if (Crc7)
    performCrc7Test();
  if (Crc8Dallas)
    performCrc8DallasTest();
  if (Crc16)
    performCrc16Test();

  printf("Passed\n");

  return 0;
}