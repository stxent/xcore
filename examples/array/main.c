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
#include <xcore/containers/array.h>
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
static void checkElements(struct Array *, size_t, bool);
static void intertwineArray(struct Array *, size_t, size_t, size_t);
static void fillArray(struct Array *, size_t, size_t, size_t);
static void reverseArray(struct Array *);
static void clearElements(struct Array *);
static void eraseElements(struct Array *, size_t, size_t, size_t);
static void popElements(struct Array *, bool);
static void printElements(struct Array *);
static void performArrayTest(void);
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
static void checkElements(struct Array *array, size_t offset, bool reverse)
{
  const size_t size = arraySize(array);

  for (size_t index = 0; index < size; ++index)
  {
    const size_t identifier = offset + (reverse ? size - index - 1 : index);
    const struct DummyStruct referenceElement = createElement(identifier);
    const struct DummyStruct * const element = arrayAt(array, index);
    const bool result = compareElements(element, &referenceElement);

    assert(result == true);

#ifdef NDEBUG
    (void)result;
#endif
  }
}
/*----------------------------------------------------------------------------*/
static void intertwineArray(struct Array *array, size_t number,
    size_t offset, size_t step)
{
  for (size_t index = 0; index < number; ++index)
  {
    const size_t position = offset + index * step;
    const struct DummyStruct element = createElement(position);

    arrayInsert(array, position, &element);
  }
}
/*----------------------------------------------------------------------------*/
static void fillArray(struct Array *array, size_t number, size_t offset,
    size_t step)
{
  for (size_t index = 0; index < number; ++index)
  {
    const size_t identifier = offset + index * step;
    const struct DummyStruct element = createElement(identifier);

    arrayPushBack(array, &element);
  }
}
/*----------------------------------------------------------------------------*/
static void reverseArray(struct Array *array)
{
  const size_t size = arraySize(array);

  for (size_t index = 0; index < size / 2; ++index)
  {
    struct DummyStruct * const front = arrayAt(array, index);
    struct DummyStruct * const back = arrayAt(array, size - index - 1);

    const struct DummyStruct buffer = *front;
    *front = *back;
    *back = buffer;
  }
}
/*----------------------------------------------------------------------------*/
static void clearElements(struct Array *array)
{
  arrayClear(array);
  assert(arraySize(array) == 0);
  assert(arrayEmpty(array) == true);
  assert(arrayFull(array) == false);
}
/*----------------------------------------------------------------------------*/
static void eraseElements(struct Array *vector, size_t number, size_t offset,
    size_t step)
{
  const size_t initialSize = arraySize(vector);

  for (size_t index = 0; index < number; ++index)
  {
    arrayErase(vector, offset + index * step);
  }

#ifdef NDEBUG
  (void)initialSize;
#endif

  assert(arraySize(vector) == initialSize - number);
}
/*----------------------------------------------------------------------------*/
static void popElements(struct Array *array, bool zero)
{
  size_t identifier = arraySize(array) - 1;

  while (arraySize(array))
  {
    if (zero)
    {
      arrayPopBack(array, 0);
    }
    else
    {
      const struct DummyStruct referenceElement = createElement(identifier);
      struct DummyStruct element;
      bool result;

      arrayPopBack(array, &element);
      result = compareElements(&element, &referenceElement);
      assert(result == true);

#ifdef NDEBUG
      (void)result;
#endif
    }

    --identifier;
  }

  assert(arraySize(array) == 0);
  assert(arrayEmpty(array) == true);
  assert(arrayFull(array) == false);
}
/*----------------------------------------------------------------------------*/
static void printElements(struct Array *array)
{
  for (size_t index = 0; index < arraySize(array); ++index)
  {
    const struct DummyStruct * const element = arrayAt(array, index);

    DEBUG_PRINT("Element %2zu: %3"PRIi64" %3"PRIi32" [",
        index, element->a, element->b);
    for (size_t entry = 0; entry < ARRAY_SIZE(element->c); ++entry)
      DEBUG_PRINT("%s%3i", entry ? " " : "", element->c[entry]);
    DEBUG_PRINT("]\n");
  }
}
/*----------------------------------------------------------------------------*/
static void performArrayTest(void)
{
  struct Array array;
  enum result res;

#ifdef NDEBUG
  (void)res;
#endif

  /* Queue initialization */
  res = arrayInit(&array, sizeof(struct DummyStruct), MAX_CAPACITY);
  assert(res == E_OK);
  assert(arrayCapacity(&array) == MAX_CAPACITY);

  /* First pass: fill, reverse, check and clear */
  fillArray(&array, MAX_CAPACITY - MAX_CAPACITY / 2, 0, 2);
  assert(arraySize(&array) == MAX_CAPACITY - MAX_CAPACITY / 2);
  assert(arrayEmpty(&array) == false);
  assert(arrayFull(&array) == false);

  intertwineArray(&array, MAX_CAPACITY / 2, 1, 2);
  assert(arraySize(&array) == MAX_CAPACITY);
  assert(arrayEmpty(&array) == false);
  assert(arrayFull(&array) == true);

  DEBUG_PRINT("First pass array:\n");
  printElements(&array);
  checkElements(&array, 0, false);

  reverseArray(&array);

  DEBUG_PRINT("First pass reversed array:\n");
  printElements(&array);
  checkElements(&array, 0, true);

  clearElements(&array);

  /* Second pass: fill, check and erase */
  fillArray(&array, MAX_CAPACITY / 2, 1, 2);
  intertwineArray(&array, MAX_CAPACITY - MAX_CAPACITY / 2, 0, 2);

  DEBUG_PRINT("Second pass array:\n");
  printElements(&array);
  checkElements(&array, 0, false);

  eraseElements(&array, MAX_CAPACITY / 2 - 1, 0, 1);

  DEBUG_PRINT("Second pass half-empty array:\n");
  printElements(&array);

  intertwineArray(&array, MAX_CAPACITY / 2 - 1, 0, 2);

  DEBUG_PRINT("Second pass restored array:\n");
  printElements(&array);
  checkElements(&array, 0, false);

  eraseElements(&array, MAX_CAPACITY, 0, 0);
  assert(arrayEmpty(&array) == true);

  /* Third pass: fill, check and pop */
  fillArray(&array, MAX_CAPACITY, 0, 1);

  DEBUG_PRINT("Third pass array:\n");
  printElements(&array);
  checkElements(&array, 0, false);

  popElements(&array, false);

  /* Fourth pass: fill, check and pop with zero argument */
  fillArray(&array, MAX_CAPACITY, 0, 1);

  DEBUG_PRINT("Fourth pass array:\n");
  printElements(&array);
  checkElements(&array, 0, false);

  popElements(&array, true);

  arrayDeinit(&array);
}
/*----------------------------------------------------------------------------*/
int main(void)
{
  performArrayTest();

  printf("Passed\n");

  return 0;
}
