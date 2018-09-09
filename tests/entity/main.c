/*
 * main.c
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <check.h>
#include <stdbool.h>
#include <stdlib.h>
#include <xcore/entity.h>
/*----------------------------------------------------------------------------*/
static const char TEST_MESSAGE[] = "Lorem ipsum";
/*----------------------------------------------------------------------------*/
struct TestObjectConfig
{
  const char *message;
};

struct TestObject
{
  struct Entity base;
  char *message;
};
/*----------------------------------------------------------------------------*/
static enum Result testClassInit(void *, const void *);
static enum Result testClassFailedInit(void *, const void *);
static void testClassDeinit(void *);
/*----------------------------------------------------------------------------*/
const struct EntityClass * const DefaultEntity = &(const struct EntityClass){
    .size = sizeof(struct TestObject),
    .init = testClassInit,
    .deinit = testClassDeinit
};

const struct EntityClass * const InconstructibleEntity =
    &(const struct EntityClass){
    .size = sizeof(struct TestObject),
    .init = testClassFailedInit,
    .deinit = testClassDeinit
};

const struct EntityClass * const UninitializedEntity =
    &(const struct EntityClass){
    .size = sizeof(struct TestObject),
    .init = 0,
    .deinit = 0
};
/*----------------------------------------------------------------------------*/
extern void *__libc_malloc(size_t);

static bool mallocHookActive = false;

void *malloc(size_t size)
{
  if (!mallocHookActive)
    return __libc_malloc(size);
  else
    return 0;
}
/*----------------------------------------------------------------------------*/
static enum Result testClassInit(void *objectBase, const void *configBase)
{
  ck_assert_ptr_ne(objectBase, 0);
  ck_assert_ptr_ne(configBase, 0);

  const struct TestObjectConfig * const config = configBase;
  struct TestObject * const object = objectBase;

  object->message = malloc(strlen(config->message) + 1);
  strcpy(object->message, config->message);

  return E_OK;
}
/*----------------------------------------------------------------------------*/
static enum Result testClassFailedInit(void *objectBase __attribute__((unused)),
    const void *configBase __attribute__((unused)))
{
  return E_ERROR;
}
/*----------------------------------------------------------------------------*/
static void testClassDeinit(void *objectBase)
{
  ck_assert_ptr_ne(objectBase, 0);

  struct TestObject * const object = objectBase;
  ck_assert_str_eq(object->message, TEST_MESSAGE);
  free(object->message);
}
/*----------------------------------------------------------------------------*/
START_TEST(testObjectCreation)
{
  static const struct TestObjectConfig config = {TEST_MESSAGE};

  /* Simply construct and delete item */
  struct TestObject * const correctObject = init(DefaultEntity, &config);
  ck_assert_ptr_ne(correctObject, 0);
  ck_assert_str_eq(correctObject->message, TEST_MESSAGE);
  deinit(correctObject);

  /* Create an object of class without constructor and destructor */
  struct TestObject * const plainObject = init(UninitializedEntity, 0);
  ck_assert_ptr_ne(plainObject, 0);
  deinit(plainObject);

  /* Fail to construct an object */
  struct TestObject * const failedObject = init(InconstructibleEntity, &config);
  ck_assert_ptr_eq(failedObject, 0);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testMemoryFailure)
{
  static const struct TestObjectConfig config = {TEST_MESSAGE};

  mallocHookActive = true;
  struct TestObject * const object = init(DefaultEntity, &config);
  mallocHookActive = false;

  ck_assert_ptr_eq(object, 0);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Entity");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testObjectCreation);
  tcase_add_test(testcase, testMemoryFailure);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
