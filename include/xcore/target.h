/*
 * xcore/target.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_TARGET_H_
#define XCORE_TARGET_H_
/*----------------------------------------------------------------------------*/
#if defined(__i386__)
#define CORE            x86
#define CORE_TYPE       x86
#elif defined(__x86_64__)
#define CORE            x86_64
#define CORE_TYPE       x86
#elif defined(__ARM_ARCH_6M__)
#define CORE            m0
#define CORE_TYPE       cortex
#elif defined(__ARM_ARCH_7M__)
#define CORE            m3
#define CORE_TYPE       cortex
#elif defined(__ARM_ARCH_7EM__)
#define CORE            m4
#define CORE_TYPE       cortex
#else
#error "Target architecture is undefined"
#endif
/*----------------------------------------------------------------------------*/
#endif /* XCORE_TARGET_H_ */
