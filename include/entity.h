/*
 * entity.h
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_ENTITY_H_
#define XCORE_ENTITY_H_
/*----------------------------------------------------------------------------*/
#include <error.h>
/*----------------------------------------------------------------------------*/
#define CLASS_HEADER \
    unsigned int size;\
    enum result (*init)(void *, const void *);\
    void (*deinit)(void *);

#define CLASS(instance) (((const struct Entity *)(instance))->descriptor)
/*----------------------------------------------------------------------------*/
struct EntityClass
{
  CLASS_HEADER
};
/*----------------------------------------------------------------------------*/
struct Entity
{
  const void *descriptor;
};
/*----------------------------------------------------------------------------*/
void *init(const void *, const void *);
void deinit(void *);
/*----------------------------------------------------------------------------*/
#endif /* XCORE_ENTITY_H_ */
