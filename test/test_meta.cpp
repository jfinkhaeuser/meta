/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.co.uk>
 *
 * Copyright (c) 2009-2012 Jens Finkhaeuser.
 * Copyright (c) 2013 Unwesen Ltd.
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
#include <vector>

#include <boost/noncopyable.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <meta/comparison.h>
#include <meta/for.h>

namespace {

static int test_sum = 0;

void
dynamic_for_functor1(int i)
{
  test_sum += i;
}


struct dynamic_for_functor2 : boost::noncopyable
{
  void operator()(int i)
  {
    test_sum += i;
  }
};


template <int CURRENT>
struct inc_twice : public meta::increment<CURRENT, 2>
{
};


template <int I>
struct static_functor
{
  void operator()()
  {
    test_sum += I;
  }


  void operator()(int factor)
  {
    test_sum += I * factor;
  }
};


} // anonymous namespace

class MetaTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(MetaTest);

      CPPUNIT_TEST(testComparison);
      CPPUNIT_TEST(testDynamicFor);
      CPPUNIT_TEST(testStaticFor);

    CPPUNIT_TEST_SUITE_END();

private:
    void testComparison()
    {
      // equal
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::equal<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::equal<42, 43>::value));

      // not_equal
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::not_equal<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::not_equal<42, 43>::value));

      // greater
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::greater<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::greater<42, 43>::value));
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::greater<43, 42>::value));

      // less
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::less<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::less<42, 43>::value));
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::less<43, 42>::value));

      // greater_equal
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::greater_equal<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::greater_equal<42, 43>::value));
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::greater_equal<43, 42>::value));

      // less_equal
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::less_equal<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::less_equal<42, 43>::value));
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::less_equal<43, 42>::value));
    }



    void testDynamicFor()
    {
      // Test both functors with default increments, i.e. inc_once
      test_sum = 0;
      {
        meta::dynamic_for<0, 10>(&dynamic_for_functor1);
        CPPUNIT_ASSERT_EQUAL(45, test_sum);
      }

      test_sum = 0;
      {
        dynamic_for_functor2 f;
        meta::dynamic_for<0, 10>(f);
        CPPUNIT_ASSERT_EQUAL(45, test_sum);
      }

      // Test both functors with increments by two.
      test_sum = 0;
      {
        meta::dynamic_for<0, 10, inc_twice>(&dynamic_for_functor1);
        CPPUNIT_ASSERT_EQUAL(20, test_sum);
      }

      test_sum = 0;
      {
        dynamic_for_functor2 f;
        meta::dynamic_for<0, 10, inc_twice>(f);
        CPPUNIT_ASSERT_EQUAL(20, test_sum);
      }

      // Test both functor with a custom comparator.
      test_sum = 0;
      {
        meta::dynamic_for<0, 10, meta::less_equal>(&dynamic_for_functor1);
        CPPUNIT_ASSERT_EQUAL(55, test_sum);
      }

      test_sum = 0;
      {
        dynamic_for_functor2 f;
        meta::dynamic_for<0, 10, meta::less_equal>(f);
        CPPUNIT_ASSERT_EQUAL(55, test_sum);
      }
    }


    void testStaticFor()
    {
      // *** Tests with default comparator
      // Test functor with default increments and no aditional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, static_functor>();
        CPPUNIT_ASSERT_EQUAL(45, test_sum);
      }

      // Test functor with increments by two and no additional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, inc_twice, static_functor>();
        CPPUNIT_ASSERT_EQUAL(20, test_sum);
      }

      // Test functor with default increments and an addtional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, static_functor>(3);
        CPPUNIT_ASSERT_EQUAL(45 * 3, test_sum);
      }

      // Test functor with increments by two and an additional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, inc_twice, static_functor>(3);
        CPPUNIT_ASSERT_EQUAL(20 * 3, test_sum);
      }

      // *** Tests with non-default comparator
      // Test functor with default increments and no aditional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, meta::less_equal, static_functor>();
        CPPUNIT_ASSERT_EQUAL(55, test_sum);
      }

      // Test functor with increments by two and no additional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, inc_twice, meta::less_equal, static_functor>();
        CPPUNIT_ASSERT_EQUAL(30, test_sum);
      }

      // Test functor with default increments and an addtional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, meta::less_equal, static_functor>(3);
        CPPUNIT_ASSERT_EQUAL(55 * 3, test_sum);
      }

      // Test functor with increments by two and an additional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, inc_twice, meta::less_equal, static_functor>(3);
        CPPUNIT_ASSERT_EQUAL(30 * 3, test_sum);
      }
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(MetaTest);
