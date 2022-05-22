/*
 * asm.c
 * Copyright (C) 2017 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/asm.h>
/*----------------------------------------------------------------------------*/
uint32_t __getMainStackPointer(void)
{
  uint32_t address;

  __asm__ volatile (
      "MRS %[address], MSP\n"
      "MOV r0, %[address]\n"
      "BX LR\n"
      : [address] "=r" (address)
  );
  return address;
}
/*----------------------------------------------------------------------------*/
uint32_t __getProcessStackPointer(void)
{
  uint32_t address;

  __asm__ volatile (
      "MRS %[address], PSP\n"
      "MOV r0, %[address]\n"
      "BX LR\n"
      : [address] "=r" (address)
  );
  return address;
}
/*----------------------------------------------------------------------------*/
void __setMainStackPointer(uint32_t address)
{
  __asm__ volatile (
      "MSR MSP, %[address]\n"
      "BX LR\n"
      :
      : [address] "r" (address)
  );
}
/*----------------------------------------------------------------------------*/
void __setProcessStackPointer(uint32_t address)
{
  __asm__ volatile (
      "MSR PSP, %[address]\n"
      "BX LR\n"
      :
      : [address] "r" (address)
  );
}
