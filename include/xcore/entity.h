/*
 * xcore/entity.h
 * Copyright (C) 2012 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_ENTITY_H_
#define XCORE_ENTITY_H_
/*----------------------------------------------------------------------------*/
#include <stddef.h>
#include <xcore/error.h>
#include <xcore/helpers.h>
/*----------------------------------------------------------------------------*/
#define CLASS_HEADER \
    size_t size;\
    enum Result (*init)(void *, const void *);\
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
BEGIN_DECLS

void *init(const void *, const void *);
void deinit(void *);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_ENTITY_H_ */
