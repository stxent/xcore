/*
 * xcore/core/cortex/accel.h
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CORE_CORTEX_ACCEL_H_
#define XCORE_CORE_CORTEX_ACCEL_H_
/*----------------------------------------------------------------------------*/
#if defined(__ARM_ARCH_6M__)
#  include "armv6m/accel.h"
#elif defined(__ARM_ARCH_7M__)
#  include "armv7m/accel.h"
#elif defined(__ARM_ARCH_7EM__)
#  include "armv7em/accel.h"
#endif
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ACCEL_H_ */
