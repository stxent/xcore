/*
 * thread.c
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/os/thread.h>
#include <limits.h>
/*----------------------------------------------------------------------------*/
static void *threadLauncher(void *object)
{
  struct Thread * const thread = object;

  /* Configure the thread */
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  /* Run user defined function */
  thread->running = true;
  thread->function(thread->functionArgument);

  return NULL;
}
/*----------------------------------------------------------------------------*/
void threadInit(struct Thread *thread, size_t size,
    int priority __attribute__((unused)), void (*function)(void *),
    void *functionArgument)
{
  thread->onTerminateCallback = NULL;
  thread->function = function;
  thread->functionArgument = functionArgument;
  thread->stackSize = size;
  thread->running = false;
}
/*----------------------------------------------------------------------------*/
void threadDeinit(struct Thread *thread)
{
  threadTerminate(thread);
}
/*----------------------------------------------------------------------------*/
void threadOnTerminateCallback(struct Thread *thread, void (*callback)(void *),
    void *callbackArgument)
{
  thread->onTerminateArgument = callbackArgument;
  thread->onTerminateCallback = callback;
}
/*----------------------------------------------------------------------------*/
enum Result threadStart(struct Thread *thread)
{
  /* Check whether the thread is already started */
  if (thread->running)
    return E_BUSY;

  pthread_attr_t attributes;
  int result = pthread_attr_init(&attributes);

  if (result == 0)
  {
    /* Set a new stack size */
    result = pthread_attr_setstacksize(&attributes,
        PTHREAD_STACK_MIN + thread->stackSize);

    if (result == 0)
    {
      /* Create and start a new thread */
      result = pthread_create(&thread->handle, &attributes,
          threadLauncher, thread);
      pthread_attr_destroy(&attributes);
    }
  }

  return result == 0 ? E_OK : E_ERROR;
}
/*----------------------------------------------------------------------------*/
void threadTerminate(struct Thread *thread)
{
  if (!thread->running)
    return;

  if (thread->onTerminateCallback != NULL)
    thread->onTerminateCallback(thread->onTerminateArgument);
  else
    pthread_cancel(thread->handle);

  pthread_join(thread->handle, NULL);
  thread->running = false;
}
/*----------------------------------------------------------------------------*/
void msleep(unsigned int interval)
{
  const struct timespec timestamp = {
      .tv_sec = interval / 1000,
      .tv_nsec = (interval % 1000) * 1000000
  };

  nanosleep(&timestamp, NULL);
}
