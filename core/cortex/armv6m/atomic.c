/*
 * atomic.c
 * Copyright (C) 2022 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/asm.h>
#include <xcore/atomic.h>
/*----------------------------------------------------------------------------*/
bool atomicCompareExchangeUL(unsigned long *pointer, unsigned long *expected,
    unsigned long desired)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  bool result;

  if (*pointer == *expected)
  {
    *pointer = desired;
    result = true;
  }
  else
  {
    *expected = *pointer;
    result = false;
  }

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
bool atomicCompareExchangeU(unsigned int *pointer, unsigned int *expected,
    unsigned int desired)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  bool result;

  if (*pointer == *expected)
  {
    *pointer = desired;
    result = true;
  }
  else
  {
    *expected = *pointer;
    result = false;
  }

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
bool atomicCompareExchangeUS(unsigned short *pointer, unsigned short *expected,
    unsigned short desired)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  bool result;

  if (*pointer == *expected)
  {
    *pointer = desired;
    result = true;
  }
  else
  {
    *expected = *pointer;
    result = false;
  }

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
bool atomicCompareExchangeUC(unsigned char *pointer, unsigned char *expected,
    unsigned char desired)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  bool result;

  if (*pointer == *expected)
  {
    *pointer = desired;
    result = true;
  }
  else
  {
    *expected = *pointer;
    result = false;
  }

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned long atomicFetchAddUL(unsigned long *pointer, unsigned long value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned long result = *pointer;
  *pointer = result + value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned int atomicFetchAddU(unsigned int *pointer, unsigned int value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned int result = *pointer;
  *pointer = result + value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned short atomicFetchAddUS(unsigned short *pointer, unsigned short value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned short result = *pointer;
  *pointer = result + value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned char atomicFetchAddUC(unsigned char *pointer, unsigned char value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned char result = *pointer;
  *pointer = result + value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned long atomicFetchAndUL(unsigned long *pointer, unsigned long value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned long result = *pointer;
  *pointer = result & value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned int atomicFetchAndU(unsigned int *pointer, unsigned int value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned int result = *pointer;
  *pointer = result & value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned short atomicFetchAndUS(unsigned short *pointer, unsigned short value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned short result = *pointer;
  *pointer = result & value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned char atomicFetchAndUC(unsigned char *pointer, unsigned char value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned char result = *pointer;
  *pointer = result & value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned long atomicFetchOrUL(unsigned long *pointer, unsigned long value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned long result = *pointer;
  *pointer = result | value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned int atomicFetchOrU(unsigned int *pointer, unsigned int value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned int result = *pointer;
  *pointer = result | value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned short atomicFetchOrUS(unsigned short *pointer, unsigned short value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned short result = *pointer;
  *pointer = result | value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned char atomicFetchOrUC(unsigned char *pointer, unsigned char value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned char result = *pointer;
  *pointer = result | value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned long atomicFetchSubUL(unsigned long *pointer, unsigned long value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned long result = *pointer;
  *pointer = result - value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned int atomicFetchSubU(unsigned int *pointer, unsigned int value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned int result = *pointer;
  *pointer = result - value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned short atomicFetchSubUS(unsigned short *pointer, unsigned short value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned short result = *pointer;
  *pointer = result - value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned char atomicFetchSubUC(unsigned char *pointer, unsigned char value)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  const unsigned char result = *pointer;
  *pointer = result - value;

  __msr_primask(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned long atomicLoadUL(/*volatile*/const unsigned long *pointer)
{
  return *pointer;
}
/*----------------------------------------------------------------------------*/
unsigned int atomicLoadU(/*volatile*/const unsigned int *pointer)
{
  return *pointer;
}
/*----------------------------------------------------------------------------*/
unsigned short atomicLoadUS(const unsigned short *pointer)
{
  return *pointer;
}
/*----------------------------------------------------------------------------*/
unsigned char atomicLoadUC(const unsigned char *pointer)
{
  return *pointer;
}
/*----------------------------------------------------------------------------*/
bool compareExchangePointer(void *pointer, void *expected, void *desired)
{
  const uint32_t state = __mrs_primask();
  __cpsid();

  bool result;

  if (*(void **)pointer == *(void **)expected)
  {
    *(void **)pointer = desired;
    result = true;
  }
  else
  {
    *(void **)expected = *(void **)pointer;
    result = false;
  }

  __msr_primask(state);
  return result;
}
