/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
 *
 * Copyright (c) 2014-2015 Unwesen Ltd.
 * Copyright (c) 2016-2017 Jens Finkhaeuser.
 *
 * This software is licensed under the terms of the GNU GPLv3 for personal,
 * educational and non-profit use. For all other uses, alternative license
 * options are available. Please contact the copyright holder for additional
 * information, stating your intended usage.
 *
 * You can find the full text of the GPLv3 in the COPYING file in this code
 * distribution.
 *
 * This software is distributed on an "AS IS" BASIS, WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 **/

#include <cppunit/extensions/HelperMacros.h>

#include <meta/math.h>


class MathTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(MathTest);

      CPPUNIT_TEST(testGCD);
      CPPUNIT_TEST(testRatio);
      CPPUNIT_TEST(testMultiplyRatio);
      CPPUNIT_TEST(testDivideRatio);

    CPPUNIT_TEST_SUITE_END();

private:

    template <typename intT>
    void testGCDImpl()
    {
      namespace m = meta::math;

      CPPUNIT_ASSERT_EQUAL(intT(5), intT(m::gcd<intT, 5, 10>::result));
      CPPUNIT_ASSERT_EQUAL(intT(5), intT(m::gcd<intT, 10, 5>::result));
    }

    void testGCD()
    {
      testGCDImpl<int8_t>();
      testGCDImpl<uint8_t>();
      testGCDImpl<int16_t>();
      testGCDImpl<uint16_t>();
      testGCDImpl<int32_t>();
      testGCDImpl<uint32_t>();
      testGCDImpl<int64_t>();
      testGCDImpl<uint64_t>();
    }



    template <typename intT>
    void testRatioImpl()
    {
      namespace m = meta::math;

      // Ratio without divisor defaults to divsor 1
      CPPUNIT_ASSERT_EQUAL(intT(1), intT(m::ratio<intT, intT(1)>::DIVIDEND));
      CPPUNIT_ASSERT_EQUAL(intT(1), intT(m::ratio<intT, intT(1)>::DIVISOR));
      CPPUNIT_ASSERT_EQUAL(intT(1), intT(m::ratio<intT, intT(1)>::GCD));

      // Ratio with divisor
      CPPUNIT_ASSERT_EQUAL(intT(1), intT(m::ratio<intT, intT(1), intT(10)>::DIVIDEND));
      CPPUNIT_ASSERT_EQUAL(intT(10), intT(m::ratio<intT, intT(1), intT(10)>::DIVISOR));
      CPPUNIT_ASSERT_EQUAL(intT(1), intT(m::ratio<intT, intT(1), intT(10)>::GCD));

      // Ratio that can be cancelled down
      CPPUNIT_ASSERT_EQUAL(intT(2), intT(m::ratio<intT, intT(6), intT(9)>::DIVIDEND));
      CPPUNIT_ASSERT_EQUAL(intT(3), intT(m::ratio<intT, intT(6), intT(9)>::DIVISOR));
      CPPUNIT_ASSERT_EQUAL(intT(3), intT(m::ratio<intT, intT(6), intT(9)>::GCD));

    }

    void testRatio()
    {
      testRatioImpl<int8_t>();
      testRatioImpl<uint8_t>();
      testRatioImpl<int16_t>();
      testRatioImpl<uint16_t>();
      testRatioImpl<int32_t>();
      testRatioImpl<uint32_t>();
      testRatioImpl<int64_t>();
      testRatioImpl<uint64_t>();
    }



    template <typename intT0, typename intT1>
    void testMultiplyRatioImpl()
    {
      namespace m = meta::math;

      typedef m::ratio<intT0, 9, 6> ratio_t1;
      typedef m::ratio<intT1, 3, 7> ratio_t2;

      typedef m::multiply_ratios<ratio_t1, ratio_t2> product_t1;
      typedef m::multiply_ratios<ratio_t2, ratio_t1> product_t2;

      // Multiplication is commutative
      CPPUNIT_ASSERT_EQUAL(int64_t(product_t1::GCD), int64_t(product_t2::GCD));
      CPPUNIT_ASSERT_EQUAL(int64_t(product_t1::DIVIDEND), int64_t(product_t2::DIVIDEND));
      CPPUNIT_ASSERT_EQUAL(int64_t(product_t1::DIVISOR), int64_t(product_t2::DIVISOR));

      // Actual values - we're just looking at the first product, and expect the didivend
      // to be the following.
      CPPUNIT_ASSERT_EQUAL(int64_t(1), int64_t(product_t1::GCD));
      CPPUNIT_ASSERT_EQUAL(int64_t(9), int64_t(product_t1::DIVIDEND));
      CPPUNIT_ASSERT_EQUAL(int64_t(14), int64_t(product_t1::DIVISOR));
    }

    void testMultiplyRatio()
    {
      // Same types
      testMultiplyRatioImpl<int8_t, int8_t>();
      testMultiplyRatioImpl<uint8_t, uint8_t>();
      testMultiplyRatioImpl<int16_t, int16_t>();
      testMultiplyRatioImpl<uint16_t, uint16_t>();
      testMultiplyRatioImpl<int32_t, int32_t>();
      testMultiplyRatioImpl<uint32_t, uint32_t>();
      testMultiplyRatioImpl<int64_t, int64_t>();
      testMultiplyRatioImpl<uint64_t, uint64_t>();

      // Some extreme types
      testMultiplyRatioImpl<int64_t, int8_t>();
      testMultiplyRatioImpl<uint8_t, uint64_t>();
    }




    template <typename intT0, typename intT1>
    void testDivideRatioImpl()
    {
      namespace m = meta::math;

      typedef m::ratio<intT0, 9, 6> ratio_t1;
      typedef m::ratio<intT1, 3, 7> ratio_t2;

      typedef m::divide_ratios<ratio_t1, ratio_t2> quotient_t1;
      typedef m::divide_ratios<ratio_t2, ratio_t1> quotient_t2;

      // Division ins non-commutative
      CPPUNIT_ASSERT_EQUAL(int64_t(quotient_t1::GCD), int64_t(quotient_t2::GCD));
      CPPUNIT_ASSERT(int64_t(quotient_t1::DIVIDEND) != int64_t(quotient_t2::DIVIDEND));
      CPPUNIT_ASSERT(int64_t(quotient_t1::DIVISOR) != int64_t(quotient_t2::DIVISOR));

      // You can, in fact, expect dividend and divisor to have switched
      CPPUNIT_ASSERT_EQUAL(int64_t(quotient_t1::DIVIDEND), int64_t(quotient_t2::DIVISOR));
      CPPUNIT_ASSERT_EQUAL(int64_t(quotient_t1::DIVISOR), int64_t(quotient_t2::DIVIDEND));

      // Actual values - we're just looking at the first quotient, and expect the didivend
      // to be the following.
      CPPUNIT_ASSERT_EQUAL(int64_t(3), int64_t(quotient_t1::GCD));
      CPPUNIT_ASSERT_EQUAL(int64_t(7), int64_t(quotient_t1::DIVIDEND));
      CPPUNIT_ASSERT_EQUAL(int64_t(2), int64_t(quotient_t1::DIVISOR));

    }

    void testDivideRatio()
    {
      // Same types
      testDivideRatioImpl<int8_t, int8_t>();
      testDivideRatioImpl<uint8_t, uint8_t>();
      testDivideRatioImpl<int16_t, int16_t>();
      testDivideRatioImpl<uint16_t, uint16_t>();
      testDivideRatioImpl<int32_t, int32_t>();
      testDivideRatioImpl<uint32_t, uint32_t>();
      testDivideRatioImpl<int64_t, int64_t>();
      testDivideRatioImpl<uint64_t, uint64_t>();

      // Some extreme types
      testDivideRatioImpl<int64_t, int8_t>();
      testDivideRatioImpl<uint8_t, uint64_t>();
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(MathTest);
