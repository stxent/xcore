/*
 * xcore/target.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_TARGET_H_
#define XCORE_TARGET_H_
/*----------------------------------------------------------------------------*/
#if defined(__i386__)
#  define CORE        x86
#  define CORE_TYPE   x86
#  define CORE_OS     posix
#elif defined(__x86_64__)
#  define CORE        x86_64
#  define CORE_TYPE   x86
#  define CORE_OS     posix
#elif defined(__ARM_ARCH_6M__)
#  define CORE        armv6m
#  define CORE_TYPE   cortex
#  define CORE_OS     generic
#elif defined(__ARM_ARCH_7M__)
#  define CORE        armv7m
#  define CORE_TYPE   cortex
#  define CORE_OS     generic
#elif defined(__ARM_ARCH_7EM__)
#  define CORE        armv7em
#  define CORE_TYPE   cortex
#  define CORE_OS     generic
#else
#  error "Target architecture is not supported"
#endif
/*----------------------------------------------------------------------------*/
#endif /* XCORE_TARGET_H_ */
