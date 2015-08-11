/*
 * unicode.c
 * Copyright (C) 2013 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <memory.h>
#include <unicode.h>
/*----------------------------------------------------------------------------*/
static const uint8_t startMark[] = {0x00, 0xC0, 0xE0, 0xF0};
/*----------------------------------------------------------------------------*/
/* Get length of the resulting string */
unsigned int uLengthFromUtf16(const char16_t *source)
{
  unsigned int length = 0;

  while (*source)
  {
    uint32_t value = (uint32_t)fromLittleEndian16(*source++);

    if ((value & 0xF800) == 0xD800)
    {
      /* Check for high surrogate */
      if (value > 0xDBFF)
        continue;

      const uint32_t surrogate = (uint32_t)fromLittleEndian16(*source++);

      /* Check for low surrogate */
      if (surrogate < 0xDC00 || surrogate > 0xDFFF)
        continue;

      value = (value & 0x03FF) << 10;
      value |= surrogate & 0x03FF;
      value += 0x10000;
    }

    uint8_t width = 0;

    if (value < 0x0080)
      width = 1;
    else if (value < 0x800)
      width = 2;
    else if (value < 0x10000)
      width = 3;
    else if (value < 0x110000)
      width = 4;

    length += width;
  }

  return length;
}
/*----------------------------------------------------------------------------*/
unsigned int uLengthToUtf16(const char *source)
{
  unsigned int length = 0;
  uint8_t value;

  while ((value = *source++))
  {
    uint8_t width = 0;

    if ((value & 0xE0) == 0xC0) /* U+0080 to U+07FF */
      width = 1;
    else if ((value & 0xF0) == 0xE0) /* U+0800 to U+FFFF */
      width = 2;
    else if ((value & 0xF8) == 0xF0) /* U+10000 to U+1FFFFF */
      width = 3;

    source += width;
    length += width < 3 ? 1 : 2;
  }

  return length;
}
/*----------------------------------------------------------------------------*/
/* Convert the string from UTF-16LE terminated with 0 to UTF-8 string */
unsigned int uFromUtf16(char *destination, const char16_t *source,
    unsigned int maxLength)
{
  unsigned int count = maxLength - 1;
  char *output = destination;

  while (count)
  {
    uint32_t value = (uint32_t)fromLittleEndian16(*source++);
    uint8_t width = 0;

    if (!value)
      break;

    if ((value & 0xF800) == 0xD800)
    {
      /* Check for high surrogate */
      if (value > 0xDBFF)
        continue;

      const uint32_t surrogate = (uint32_t)fromLittleEndian16(*source++);

      /* Check for low surrogate */
      if (surrogate < 0xDC00 || surrogate > 0xDFFF)
        continue;

      value = (value & 0x03FF) << 10;
      value |= surrogate & 0x03FF;
      value += 0x10000;
    }

    if (value < 0x0080)
      width = 1;
    else if (value < 0x800)
      width = 2;
    else if (value < 0x10000)
      width = 3;
    else if (value < 0x110000)
      width = 4;

    if (count < width)
      break;

    char *end = output + width;

    switch (width)
    {
      case 4:
        *--end = (value | 0x80) & 0xBF;
        value >>= 6;
      case 3:
        *--end = (value | 0x80) & 0xBF;
        value >>= 6;
      case 2:
        *--end = (value | 0x80) & 0xBF;
        value >>= 6;
      case 1:
        *--end = value | startMark[width - 1];
        break;
    }

    count -= width;
    output += width;
  }
  *output = '\0';

  return (unsigned int)(output - destination);
}
/*----------------------------------------------------------------------------*/
/* Convert the string from UTF-8 terminated with 0 to UTF-16LE string */
unsigned int uToUtf16(char16_t *destination, const char *source,
    unsigned int maxLength)
{
  unsigned int count = 0;
  uint8_t value;

  while ((value = *source++) && count < maxLength - 1)
  {
    uint8_t width = 0;

    if ((value & 0xE0) == 0xC0) /* U+0080 to U+07FF */
      width = 1;
    else if ((value & 0xF0) == 0xE0) /* U+0800 to U+FFFF */
      width = 2;
    else if ((value & 0xF8) == 0xF0) /* U+10000 to U+1FFFFF */
      width = 3;

    uint32_t code = (uint32_t)(value & ~startMark[width]) << (6 * width);

    while (width--)
      code |= (uint32_t)(*source++ & 0x3F) << (6 * width);

    if (code < 0x10000)
    {
      *destination++ = toLittleEndian16((uint16_t)code);
      ++count;
    }
    else
    {
      uint16_t surrogate;

      code -= 0x10000;
      surrogate = 0xD800 | (uint16_t)((code >> 10) & 0x03FF);
      *destination++ = toLittleEndian16(surrogate);
      surrogate = 0xDC00 | (uint16_t)(code & 0x03FF);
      *destination++ = toLittleEndian16(surrogate);
      count += 2;
    }
  }
  *destination = '\0';

  return count;
}
