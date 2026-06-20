/*
 * xcore/core/cortex/armv6m/accel.h
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_CORTEX_ACCEL_H_
#error This header should not be included directly
#endif

#ifndef XCORE_CORE_CORTEX_ARMV6M_ACCEL_H_
#define XCORE_CORE_CORTEX_ARMV6M_ACCEL_H_
/*----------------------------------------------------------------------------*/
#include "../../generic/accel_clz.h"
#include "../../generic/accel_rev.h"
#include "../../generic/accel_sat.h"
#include <xcore/asm.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline void invokeDebugger(void)
{
  __bkpt();
}

static inline void invokeSupervisor(void)
{
  __svc();
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_CORTEX_ARMV6M_ACCEL_H_ */
