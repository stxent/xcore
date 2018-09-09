/*
 * xcore/containers/tg_array.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CONTAINERS_TG_ARRAY_H_
#define XCORE_CONTAINERS_TG_ARRAY_H_
/*----------------------------------------------------------------------------*/
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
#define DEFINE_ARRAY(type, name, prefix) \
    typedef struct \
    { \
      type *data; \
      size_t capacity; \
      size_t size; \
    } name##Array; \
    \
    static inline type *prefix##ArrayAt(name##Array *array, size_t index) \
    { \
      assert(array->size > index); \
      return array->data + index; \
    } \
    \
    static inline type prefix##ArrayBack(const name##Array *array) \
    { \
      assert(array->size > 0); \
      return array->data[array->size - 1]; \
    } \
    \
    static inline size_t prefix##ArrayCapacity(const name##Array *array) \
    { \
      return array->capacity; \
    } \
    \
    static inline void prefix##ArrayClear(name##Array *array) \
    { \
      array->size = 0; \
    } \
    \
    static inline bool prefix##ArrayEmpty(const name##Array *array) \
    { \
      return array->size == 0; \
    } \
    \
    static inline void prefix##ArrayErase(name##Array *array, size_t index) \
    { \
      assert(index < array->size);\
      \
      --array->size; \
      memmove(array->data + index, array->data + index + 1, \
          (array->size - index) * sizeof(type)); \
    } \
    \
    static inline bool prefix##ArrayFull(const name##Array *array) \
    { \
      return array->size == array->capacity; \
    } \
    \
    static inline void prefix##ArrayInsert(name##Array *array, size_t before, \
        type element) \
    { \
      assert(before <= array->size); \
      assert(array->size < array->capacity); \
      \
      memmove(array->data + before + 1, array->data + before, \
          (array->size - before) * sizeof(type)); \
      array->data[before] = element; \
      \
      ++array->size; \
    } \
    \
    static inline void prefix##ArrayPopBack(name##Array *array) \
    { \
      assert(array->size > 0); \
      --array->size; \
    } \
    \
    static inline void prefix##ArrayPushBack(name##Array *array, type element) \
    { \
      assert(array->size < array->capacity); \
      \
      array->data[array->size] = element; \
      ++array->size; \
    }\
    \
    static inline size_t prefix##ArraySize(const name##Array *array) \
    { \
      return array->size; \
    } \
    \
    static inline bool prefix##ArrayInit(name##Array *array, size_t capacity) \
    { \
      array->data = malloc(capacity * sizeof(type)); \
      \
      if (array->data) \
      { \
        array->capacity = capacity; \
        array->size = 0; \
        return true; \
      } \
      else \
      { \
        return false; \
      } \
    } \
    \
    static inline void prefix##ArrayDeinit(name##Array *array) \
    { \
      free(array->data); \
    }
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CONTAINERS_TG_ARRAY_H_ */
