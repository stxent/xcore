/*
 * main.c
 * Copyright (C) 2026 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/accel.h>
#include <xcore/memory.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
START_TEST(testBitReverse)
{
  static const uint32_t sourceValue32 = 0x01234567UL;
  static const uint32_t targetValue32 = 0xE6A2C480UL;

  ck_assert_uint_eq(reverseBits32(sourceValue32), targetValue32);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testByteReverse)
{
  static const uint16_t sourceValue16 = 0xABCDU;
  static const uint32_t sourceValue32 = 0x01234567UL;
  static const uint64_t sourceValue64 = 0x0123456789ABCDEFULL;
  static const uint16_t targetValue16 = 0xCDABU;
  static const uint32_t targetValue32 = 0x67452301UL;
  static const uint64_t targetValue64 = 0xEFCDAB8967452301ULL;

#if __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__
  ck_assert_uint_eq(fromBigEndian16(sourceValue16), targetValue16);
  ck_assert_uint_eq(fromBigEndian32(sourceValue32), targetValue32);
  ck_assert_uint_eq(fromBigEndian64(sourceValue64), targetValue64);
  ck_assert_uint_eq(fromLittleEndian16(sourceValue16), sourceValue16);
  ck_assert_uint_eq(fromLittleEndian32(sourceValue32), sourceValue32);
  ck_assert_uint_eq(fromLittleEndian64(sourceValue64), sourceValue64);
  ck_assert_uint_eq(toBigEndian16(sourceValue16), targetValue16);
  ck_assert_uint_eq(toBigEndian32(sourceValue32), targetValue32);
  ck_assert_uint_eq(toBigEndian64(sourceValue64), targetValue64);
  ck_assert_uint_eq(toLittleEndian16(sourceValue16), sourceValue16);
  ck_assert_uint_eq(toLittleEndian32(sourceValue32), sourceValue32);
  ck_assert_uint_eq(toLittleEndian64(sourceValue64), sourceValue64);
#else
  ck_assert_uint_eq(fromBigEndian16(sourceValue16), sourceValue16);
  ck_assert_uint_eq(fromBigEndian32(sourceValue32), sourceValue32);
  ck_assert_uint_eq(fromBigEndian64(sourceValue64), sourceValue64);
  ck_assert_uint_eq(fromLittleEndian16(sourceValue16), targetValue16);
  ck_assert_uint_eq(fromLittleEndian32(sourceValue32), targetValue32);
  ck_assert_uint_eq(fromLittleEndian64(sourceValue64), targetValue64);
  ck_assert_uint_eq(toBigEndian16(sourceValue16), sourceValue16);
  ck_assert_uint_eq(toBigEndian32(sourceValue32), sourceValue32);
  ck_assert_uint_eq(toBigEndian64(sourceValue64), sourceValue64);
  ck_assert_uint_eq(toLittleEndian16(sourceValue16), targetValue16);
  ck_assert_uint_eq(toLittleEndian32(sourceValue32), targetValue32);
  ck_assert_uint_eq(toLittleEndian64(sourceValue64), targetValue64);
#endif
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Accel");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testBitReverse);
  tcase_add_test(testcase, testByteReverse);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
