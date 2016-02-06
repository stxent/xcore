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
#ifdef CONFIG_DEBUG
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
const struct CrcEngineClass * const Crc16CCITT;
const struct CrcEngineClass * const Crc32;
/*----------------------------------------------------------------------------*/
static void performCrc7Test(void)
{
  const char * const name = "CRC-7";
  const uint32_t initial = 0;
  struct CrcEngine *engine;
  uint32_t result;

  DEBUG_PRINT("Test %s\n", name);

  engine = init(Crc7, 0);
  assert(engine != 0);

  result = crcUpdate(engine, initial, (const uint8_t *)emptyString,
      strlen(emptyString));
  DEBUG_PRINT("%s of empty string: 0x%02X\n", name, result);
  assert(result == 0x00);

  result = crcUpdate(engine, initial, (const uint8_t *)dataString,
      strlen(dataString));
  DEBUG_PRINT("%s of data string: 0x%02X\n", name, result);
  assert(result == 0x0F);

  deinit(engine);
  (void)result;
}
/*----------------------------------------------------------------------------*/
static void performCrc8DallasTest(void)
{
  const char * const name = "CRC-8-Dallas";
  const uint32_t initial = 0;
  struct CrcEngine *engine;
  uint32_t result;

  DEBUG_PRINT("Test %s\n", name);

  engine = init(Crc8Dallas, 0);
  assert(engine != 0);

  result = crcUpdate(engine, initial, (const uint8_t *)emptyString,
      strlen(emptyString));
  DEBUG_PRINT("%s of empty string: 0x%02X\n", name, result);
  assert(result == 0x00);

  result = crcUpdate(engine, initial, (const uint8_t *)dataString,
      strlen(dataString));
  DEBUG_PRINT("%s of data string: 0x%02X\n", name, result);
  assert(result == 0x3B);

  deinit(engine);
  (void)result;
}
/*----------------------------------------------------------------------------*/
static void performCrc16CCITTTest(void)
{
  const char * const name = "CRC-16-CCITT";
  const uint32_t initial = 0xFFFF;
  struct CrcEngine *engine;
  uint32_t result;

  DEBUG_PRINT("Test %s\n", name);

  engine = init(Crc16CCITT, 0);
  assert(engine != 0);

  result = crcUpdate(engine, initial, (const uint8_t *)emptyString,
      strlen(emptyString));
  DEBUG_PRINT("%s of empty string: 0x%04X\n", name, result);
  assert(result == 0xFFFF);

  result = crcUpdate(engine, initial, (const uint8_t *)dataString,
      strlen(dataString));
  DEBUG_PRINT("%s of data string: 0x%04X\n", name, result);
  assert(result == 0x6472);

  deinit(engine);
  (void)result;
}
/*----------------------------------------------------------------------------*/
static void performCrc32Test(void)
{
  const char * const name = "CRC-32";
  const uint32_t initial = 0;
  struct CrcEngine *engine;
  uint32_t result;

  DEBUG_PRINT("Test %s\n", name);

  engine = init(Crc32, 0);
  assert(engine != 0);

  result = crcUpdate(engine, initial, (const uint8_t *)emptyString,
      strlen(emptyString));
  DEBUG_PRINT("%s of empty string: 0x%08X\n", name, result);
  assert(result == 0x00000000);

  result = crcUpdate(engine, initial, (const uint8_t *)dataString,
      strlen(dataString));
  DEBUG_PRINT("%s of data string: 0x%08X\n", name, result);
  assert(result == 0xC268A8E6);

  deinit(engine);
  (void)result;
}
/*----------------------------------------------------------------------------*/
int main(void)
{
  DEBUG_PRINT("CRC-7 module location: 0x%08lX\n", (unsigned long)Crc7);
  DEBUG_PRINT("CRC-8-Dallas module location: 0x%08lX\n",
      (unsigned long)Crc8Dallas);
  DEBUG_PRINT("CRC-16-CCITT module location: 0x%08lX\n",
      (unsigned long)Crc16CCITT);
  DEBUG_PRINT("CRC-32 module location: 0x%08lX\n", (unsigned long)Crc32);

  if (Crc7)
    performCrc7Test();
  if (Crc8Dallas)
    performCrc8DallasTest();
  if (Crc16CCITT)
    performCrc16CCITTTest();
  if (Crc32)
    performCrc32Test();

  printf("Passed\n");

  return 0;
}
