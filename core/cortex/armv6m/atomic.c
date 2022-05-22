/*
 * atomic.c
 * Copyright (C) 2022 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/asm.h>
#include <xcore/atomic.h>
/*----------------------------------------------------------------------------*/
unsigned long atomicFetchAddUL(volatile unsigned long *pointer,
    unsigned long value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned long result = *pointer;
  *pointer = result + value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned int atomicFetchAddU(volatile unsigned int *pointer,
    unsigned int value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned int result = *pointer;
  *pointer = result + value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned short atomicFetchAddUS(volatile unsigned short *pointer,
    unsigned short value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned short result = *pointer;
  *pointer = result + value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned char atomicFetchAddUC(volatile unsigned char *pointer,
    unsigned char value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned char result = *pointer;
  *pointer = result + value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned long atomicFetchAndUL(volatile unsigned long *pointer,
    unsigned long value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned long result = *pointer;
  *pointer = result & value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned int atomicFetchAndU(volatile unsigned int *pointer,
    unsigned int value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned int result = *pointer;
  *pointer = result & value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned short atomicFetchAndUS(volatile unsigned short *pointer,
    unsigned short value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned short result = *pointer;
  *pointer = result & value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned char atomicFetchAndUC(volatile unsigned char *pointer,
    unsigned char value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned char result = *pointer;
  *pointer = result & value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned long atomicFetchOrUL(volatile unsigned long *pointer,
    unsigned long value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned long result = *pointer;
  *pointer = result | value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned int atomicFetchOrU(volatile unsigned int *pointer,
    unsigned int value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned int result = *pointer;
  *pointer = result | value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned short atomicFetchOrUS(volatile unsigned short *pointer,
    unsigned short value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned short result = *pointer;
  *pointer = result | value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned char atomicFetchOrUC(volatile unsigned char *pointer,
    unsigned char value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned char result = *pointer;
  *pointer = result | value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned long atomicFetchSubUL(volatile unsigned long *pointer,
    unsigned long value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned long result = *pointer;
  *pointer = result - value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned int atomicFetchSubU(volatile unsigned int *pointer,
    unsigned int value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned int result = *pointer;
  *pointer = result - value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned short atomicFetchSubUS(volatile unsigned short *pointer,
    unsigned short value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned short result = *pointer;
  *pointer = result - value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
unsigned char atomicFetchSubUC(volatile unsigned char *pointer,
    unsigned char value)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

  const unsigned char result = *pointer;
  *pointer = result - value;

  __interruptsSetState(state);
  return result;
}
/*----------------------------------------------------------------------------*/
bool compareExchangePointer(volatile void *pointer, void *expected,
    void *desired)
{
  const uint32_t state = __interruptsGetState();
  __interruptsDisable();

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

  __interruptsSetState(state);
  return result;
}
