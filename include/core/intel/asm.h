/*
 * core/intel/asm.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_INTEL_ASM_H_
#define XCORE_CORE_INTEL_ASM_H_
/*----------------------------------------------------------------------------*/
#define barrier() __asm__ volatile ("" : : : "memory")
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_INTEL_ASM_H_ */
