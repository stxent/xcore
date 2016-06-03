/*
 * main.c
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <xcore/bits.h>
#include <xcore/containers/vector.h>
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif
/*----------------------------------------------------------------------------*/
#define MAX_CAPACITY 17
/*----------------------------------------------------------------------------*/
struct DummyStruct
{
  int64_t a;
  int32_t b;
  int16_t c[6];
};
/*----------------------------------------------------------------------------*/
static bool compareElements(const struct DummyStruct *,
    const struct DummyStruct *);
struct DummyStruct createElement(size_t);
static void checkElements(struct Vector *, size_t, bool);
static void intertwineVector(struct Vector *, size_t, size_t, size_t);
static void fillVector(struct Vector *, size_t, size_t, size_t);
static void reverseVector(struct Vector *);
static void clearElements(struct Vector *);
static void eraseElements(struct Vector *, size_t, size_t, size_t);
static void popElements(struct Vector *, bool);
static void printElements(struct Vector *);
static void performVectorTest(void);
/*----------------------------------------------------------------------------*/
static bool compareElements(const struct DummyStruct *a,
    const struct DummyStruct *b)
{
  if (a->a != b->a)
    return false;
  else if (a->b != b->b)
    return false;
  else if (memcmp(a->c, b->c, sizeof(a->c)))
    return false;
  else
    return true;
}
/*----------------------------------------------------------------------------*/
struct DummyStruct createElement(size_t index)
{
  const struct DummyStruct element = {
      .a = index * 2,
      .b = -index * 3,
      .c = {-index, index + 1, -index + 2, index + 3, -index + 4, index + 5}
  };

  return element;
}
/*----------------------------------------------------------------------------*/
static void checkElements(struct Vector *vector, size_t offset, bool reverse)
{
  const size_t size = vectorSize(vector);

  for (size_t index = 0; index < size; ++index)
  {
    const size_t identifier = offset + (reverse ? size - index - 1 : index);
    const struct DummyStruct referenceElement = createElement(identifier);
    const struct DummyStruct * const element = vectorAt(vector, index);
    const bool result = compareElements(element, &referenceElement);

    assert(result == true);

#ifdef NDEBUG
    (void)result;
#endif
  }
}
/*----------------------------------------------------------------------------*/
static void intertwineVector(struct Vector *vector, size_t number,
    size_t offset, size_t step)
{
  for (size_t index = 0; index < number; ++index)
  {
    const size_t position = offset + index * step;
    const struct DummyStruct element = createElement(position);

    vectorInsert(vector, position, &element);
  }
}
/*----------------------------------------------------------------------------*/
static void fillVector(struct Vector *vector, size_t number, size_t offset,
    size_t step)
{
  for (size_t index = 0; index < number; ++index)
  {
    const size_t identifier = offset + index * step;
    const struct DummyStruct element = createElement(identifier);

    vectorPushBack(vector, &element);
  }
}
/*----------------------------------------------------------------------------*/
static void reverseVector(struct Vector *vector)
{
  const size_t size = vectorSize(vector);

  for (size_t index = 0; index < size / 2; ++index)
  {
    struct DummyStruct * const front = vectorAt(vector, index);
    struct DummyStruct * const back = vectorAt(vector, size - index - 1);

    const struct DummyStruct buffer = *front;
    *front = *back;
    *back = buffer;
  }
}
/*----------------------------------------------------------------------------*/
static void clearElements(struct Vector *vector)
{
  vectorClear(vector);
  assert(vectorSize(vector) == 0);
  assert(vectorEmpty(vector) == true);
  assert(vectorFull(vector) == false);
}
/*----------------------------------------------------------------------------*/
static void eraseElements(struct Vector *vector, size_t number, size_t offset,
    size_t step)
{
  const size_t initialSize = vectorSize(vector);

  for (size_t index = 0; index < number; ++index)
  {
    vectorErase(vector, offset + index * step);
  }

#ifdef NDEBUG
  (void)initialSize;
#endif

  assert(vectorSize(vector) == initialSize - number);
}
/*----------------------------------------------------------------------------*/
static void popElements(struct Vector *vector, bool zero)
{
  size_t identifier = vectorSize(vector) - 1;

  while (vectorSize(vector))
  {
    if (zero)
    {
      vectorPopBack(vector, 0);
    }
    else
    {
      const struct DummyStruct referenceElement = createElement(identifier);
      struct DummyStruct element;
      bool result;

      vectorPopBack(vector, &element);
      result = compareElements(&element, &referenceElement);
      assert(result == true);

#ifdef NDEBUG
      (void)result;
#endif
    }

    --identifier;
  }

  assert(vectorSize(vector) == 0);
  assert(vectorEmpty(vector) == true);
  assert(vectorFull(vector) == false);
}
/*----------------------------------------------------------------------------*/
static void printElements(struct Vector *vector)
{
  for (size_t index = 0; index < vectorSize(vector); ++index)
  {
    const struct DummyStruct * const element = vectorAt(vector, index);

    DEBUG_PRINT("Element %2zu: %3"PRIi64" %3"PRIi32" [",
        index, element->a, element->b);
    for (size_t entry = 0; entry < ARRAY_SIZE(element->c); ++entry)
      DEBUG_PRINT("%s%3i", entry ? " " : "", element->c[entry]);
    DEBUG_PRINT("]\n");
  }
}
/*----------------------------------------------------------------------------*/
static void performVectorTest(void)
{
  struct Vector vector;
  enum result res;

#ifdef NDEBUG
  (void)res;
#endif

  /* Queue initialization */
  res = vectorInit(&vector, sizeof(struct DummyStruct), MAX_CAPACITY);
  assert(res == E_OK);
  assert(vectorCapacity(&vector) == MAX_CAPACITY);

  /* First pass: fill, reverse, check and clear */
  fillVector(&vector, MAX_CAPACITY - MAX_CAPACITY / 2, 0, 2);
  assert(vectorSize(&vector) == MAX_CAPACITY - MAX_CAPACITY / 2);
  assert(vectorEmpty(&vector) == false);
  assert(vectorFull(&vector) == false);

  intertwineVector(&vector, MAX_CAPACITY / 2, 1, 2);
  assert(vectorSize(&vector) == MAX_CAPACITY);
  assert(vectorEmpty(&vector) == false);
  assert(vectorFull(&vector) == true);

  DEBUG_PRINT("First pass vector:\n");
  printElements(&vector);
  checkElements(&vector, 0, false);

  reverseVector(&vector);

  DEBUG_PRINT("First pass reversed vector:\n");
  printElements(&vector);
  checkElements(&vector, 0, true);

  clearElements(&vector);

  /* Second pass: fill, check and erase */
  fillVector(&vector, MAX_CAPACITY / 2, 1, 2);
  intertwineVector(&vector, MAX_CAPACITY - MAX_CAPACITY / 2, 0, 2);

  DEBUG_PRINT("Second pass vector:\n");
  printElements(&vector);
  checkElements(&vector, 0, false);

  eraseElements(&vector, MAX_CAPACITY / 2 - 1, 0, 1);

  DEBUG_PRINT("Second pass half-empty vector:\n");
  printElements(&vector);

  intertwineVector(&vector, MAX_CAPACITY / 2 - 1, 0, 2);

  DEBUG_PRINT("Second pass restored vector:\n");
  printElements(&vector);
  checkElements(&vector, 0, false);

  eraseElements(&vector, MAX_CAPACITY, 0, 0);
  assert(vectorEmpty(&vector) == true);

  /* Third pass: fill, check and pop */
  fillVector(&vector, MAX_CAPACITY, 0, 1);

  DEBUG_PRINT("Third pass vector:\n");
  printElements(&vector);
  checkElements(&vector, 0, false);

  popElements(&vector, false);

  /* Fourth pass: fill, check and pop with zero argument */
  fillVector(&vector, MAX_CAPACITY, 0, 1);

  DEBUG_PRINT("Fourth pass vector:\n");
  printElements(&vector);
  checkElements(&vector, 0, false);

  popElements(&vector, true);

  vectorDeinit(&vector);
}
/*----------------------------------------------------------------------------*/
int main(void)
{
  performVectorTest();

  printf("Passed\n");

  return 0;
}
