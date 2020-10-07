/*
 * xcore/core/cortex/asm.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_CORTEX_ASM_H_
#define XCORE_CORE_CORTEX_ASM_H_
/*----------------------------------------------------------------------------*/
#define barrier() __asm__ volatile ("" : : : "memory")
/*----------------------------------------------------------------------------*/
#if defined(__ARM_ARCH_6M__)
#  include "armv6m/asm.h"
#elif defined(__ARM_ARCH_7M__)
#  include "armv7m/asm.h"
#elif defined(__ARM_ARCH_7EM__)
#  include "armv7em/asm.h"
#endif
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ASM_H_ */
