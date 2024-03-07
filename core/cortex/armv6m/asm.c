/*
 * asm.c
 * Copyright (C) 2017 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/asm.h>
/*----------------------------------------------------------------------------*/
uint32_t __getMainStackPointer(void)
{
  __asm__ volatile (
      "MRS r0, MSP\n"
      "BX LR\n"
  );
}
/*----------------------------------------------------------------------------*/
uint32_t __getProcessStackPointer(void)
{
  __asm__ volatile (
      "MRS r0, PSP\n"
      "BX LR\n"
  );
}
/*----------------------------------------------------------------------------*/
void __setMainStackPointer(uint32_t)
{
  __asm__ volatile (
      "MSR MSP, r0\n"
      "BX LR\n"
  );
}
/*----------------------------------------------------------------------------*/
void __setProcessStackPointer(uint32_t)
{
  __asm__ volatile (
      "MSR PSP, r0\n"
      "BX LR\n"
  );
}
