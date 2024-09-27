/*
 * xcore/core/x86/asm.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_ASM_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_X86_ASM_H_
#define XCORE_CORE_X86_ASM_H_
/*----------------------------------------------------------------------------*/
#define barrier() __asm__ volatile ("" ::: "memory")
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_X86_ASM_H_ */
