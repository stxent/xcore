/*
 * core/intel/asm.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef CORE_INTEL_ASM_H_
#define CORE_INTEL_ASM_H_
/*----------------------------------------------------------------------------*/
#define barrier() __asm__ volatile ("" : : : "memory")
/*----------------------------------------------------------------------------*/
#endif /* CORE_INTEL_ASM_H_ */
