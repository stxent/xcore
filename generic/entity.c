/*
 * entity.c
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/entity.h>
#include <assert.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
void deletedDestructorTrap(void *) __attribute__((weak));
/*----------------------------------------------------------------------------*/
void *init(const void *descriptor, const void *arguments)
{
  const struct EntityClass * const base = descriptor;

  assert(base != 0);
  assert(base->size != 0);

  struct Entity *entity = malloc(base->size);

  if (entity)
  {
    entity->descriptor = base;

    if (base->init && base->init(entity, arguments) != E_OK)
    {
      free(entity);
      entity = 0;
    }
  }

  return entity;
}
/*----------------------------------------------------------------------------*/
void deinit(void *entity)
{
  const struct EntityClass * const base = CLASS(entity);

  assert(base != 0);

  if (base->deinit)
    base->deinit(entity);

  free(entity);
}
/*----------------------------------------------------------------------------*/
void deletedDestructorTrap(void *entity __attribute__((unused)))
{
  exit(EXIT_FAILURE);
}
