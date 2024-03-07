/*
 * entity.c
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/entity.h>
#include <assert.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
[[gnu::weak]] void deletedDestructorTrap(void *);
/*----------------------------------------------------------------------------*/
void *init(const void *descriptor, const void *arguments)
{
  const struct EntityClass * const base = descriptor;

  assert(base != NULL);
  assert(base->size != 0);

  struct Entity *entity = malloc(base->size);

  if (entity != NULL)
  {
    entity->descriptor = base;

    if (base->init != NULL && base->init(entity, arguments) != E_OK)
    {
      free(entity);
      entity = NULL;
    }
  }

  return entity;
}
/*----------------------------------------------------------------------------*/
void deinit(void *entity)
{
  const struct EntityClass * const base = CLASS(entity);

  assert(base != NULL);

  if (base->deinit != NULL)
    base->deinit(entity);

  free(entity);
}
/*----------------------------------------------------------------------------*/
void deletedDestructorTrap(void *entity __attribute__((unused)))
{
  exit(EXIT_FAILURE);
}
