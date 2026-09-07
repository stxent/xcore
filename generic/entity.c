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

  assert(base != nullptr);
  assert(base->size != 0);

  struct Entity *entity = malloc(base->size);

  if (entity != nullptr)
  {
    entity->descriptor = base;

    if (base->init != nullptr && base->init(entity, arguments) != E_OK)
    {
      free(entity);
      entity = nullptr;
    }
  }

  return entity;
}
/*----------------------------------------------------------------------------*/
void deinit(void *entity)
{
  const struct EntityClass * const base = CLASS(entity);

  assert(base != nullptr);

  if (base->deinit != nullptr)
    base->deinit(entity);

  free(entity);
}
/*----------------------------------------------------------------------------*/
void deletedDestructorTrap(void *)
{
  exit(EXIT_FAILURE);
}
