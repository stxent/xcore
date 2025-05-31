/*
 * xcore/os/generic/mutex.h
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_OS_GENERIC_MUTEX_H_
#define XCORE_OS_GENERIC_MUTEX_H_
/*----------------------------------------------------------------------------*/
#include <xcore/error.h>
#include <xcore/helpers.h>
/*----------------------------------------------------------------------------*/
struct Mutex
{
  void *data;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

enum Result mutexInit(struct Mutex *);
void mutexDeinit(struct Mutex *);
void mutexLock(struct Mutex *);
bool mutexTryLock(struct Mutex *, unsigned int);
void mutexUnlock(struct Mutex *);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_OS_GENERIC_MUTEX_H_ */
