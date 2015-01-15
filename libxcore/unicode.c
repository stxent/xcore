/*
 * unicode.c
 * Copyright (C) 2013 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <memory.h>
#include <unicode.h>
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_UNICODE_DEBUG
#include <stdio.h>
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_UNICODE_DEBUG
static void dumpChar16String(const char16_t *array)
{
  while (*array)
    DEBUG_PRINT("%04X ", (unsigned int)(*array++) & 0xFFFF);

  DEBUG_PRINT("\n");
}
#endif
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_UNICODE_DEBUG
static void dumpChar8String(const char *array)
{
  while (*array)
    DEBUG_PRINT("%02X ", (unsigned int)(*array++) & 0xFF);

  DEBUG_PRINT("\n");
}
#endif
/*----------------------------------------------------------------------------*/
/* TODO Add surrogate pairs support */
/* Convert the string from UTF-16LE terminated with 0 to UTF-8 string */
uint16_t uFromUtf16(char *destination, const char16_t *source,
    uint16_t maxLength)
{
  char16_t value;
  char *output = destination;

#ifdef CONFIG_UNICODE_DEBUG
  dumpChar16String(source);
#endif

  while ((value = *source++) && output - destination < maxLength - 1)
  {
    value = fromLittleEndian16(value);

    if (value <= 0x007F)
    {
      *output++ = (char)value;
    }
    else if (value >= 0x0080 && value <= 0x07FF)
    {
      *output++ = (char)(0xC0 | (value >> 6));
      *output++ = (char)(0x80 | (value & 0x003F));
    }
    else
    {
      *output++ = (char)(0xE0 | (value >> 12));
      *output++ = (char)(0x80 | ((value >> 6) & 0x003F));
      *output++ = (char)(0x80 | (value & 0x003F));
    }
  }
  *output = '\0';

#ifdef CONFIG_UNICODE_DEBUG
  dumpChar8String(destination);
#endif

  return (uint16_t)(output - destination);
}
/*----------------------------------------------------------------------------*/
/* TODO Add surrogate pairs support */
/* Convert the string from UTF-8 terminated with 0 to UTF-16LE string */
uint16_t uToUtf16(char16_t *destination, const char *source, uint16_t maxLength)
{
  uint16_t count = 0;
  char16_t code;
  uint8_t value;

  while ((value = *source++) && count < maxLength - 1)
  {
    if (!(value & 0x80)) /* U+007F */
    {
      *destination++ = (char16_t)value;
      count++;
    }
    if ((value & 0xE0) == 0xC0) /* U+07FF */
    {
      code = (char16_t)(value & 0x1F) << 6;
      code |= (char16_t)(*source++ & 0x3F);
      *destination++ = code;
      count++;
    }
    if ((value & 0xF0) == 0xE0) /* U+FFFF */
    {
      code = (char16_t)(value & 0x0F) << 12;
      code |= (char16_t)(*source++ & 0x3F) << 6;
      code |= (char16_t)(*source++ & 0x3F);
      *destination++ = code;
      count++;
    }
  }
  *destination = '\0';
  return count;
}
