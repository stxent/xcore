/*
 * xcore/os/generic/semaphore.h
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_OS_GENERIC_SEMAPHORE_H_
#define XCORE_OS_GENERIC_SEMAPHORE_H_
/*----------------------------------------------------------------------------*/
#include <xcore/error.h>
#include <xcore/helpers.h>
#include <stdbool.h>
/*----------------------------------------------------------------------------*/
struct Semaphore
{
  void *data;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

enum Result semInit(struct Semaphore *, int);
void semDeinit(struct Semaphore *);
void semPost(struct Semaphore *);
bool semTryWait(struct Semaphore *, unsigned int);
int semValue(struct Semaphore *);
void semWait(struct Semaphore *);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_OS_GENERIC_SEMAPHORE_H_ */
