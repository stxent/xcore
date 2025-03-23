/*
 * xcore/core/riscv/csr.h
 * Copyright (C) 2025 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CORE_RISCV_CSR_H_
#define XCORE_CORE_RISCV_CSR_H_
/*----------------------------------------------------------------------------*/
#include <xcore/asm.h>
/*----------------------------------------------------------------------------*/
#define csrClear(number, value)         __csrc(number, value)
#define csrRead(number)                 __csrr(number)
#define csrReadAndClear(number, value)  __csrrc(number, value)
#define csrReadAndSet(number, value)    __csrrs(number, value)
#define csrSet(number, value)           __csrs(number, value)
#define csrSwap(number, value)          __csrrw(number, value)
#define csrWrite(number, value)         __csrw(number, value)
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CORE_RISCV_CSR_H_ */
