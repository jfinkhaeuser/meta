/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.co.uk>
 *
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

#include <cppunit/extensions/HelperMacros.h>

#include <meta/condition.h>

namespace {

// Simple conditions
struct _true
{
  static bool check()
  {
    return true;
  }
};


struct _false
{
  static bool check()
  {
    return false;
  }
};


// Parametrized conditions
struct less
{
  template <typename T1, typename T2>
  static bool check(T1 const & t1, T2 const & t2)
  {
    return (t1 < t2);
  }
};


struct more
{
  template <typename T1, typename T2>
  static bool check(T1 const & t1, T2 const & t2)
  {
    return (t1 > t2);
  }
};

// Simple conditions that ignore parameters
struct _true_ignore_params
{
  template <typename... Args>
  static bool check(Args...)
  {
    return true;
  }
};

struct _false_ignore_params
{
  template <typename... Args>
  static bool check(Args...)
  {
    return false;
  }
};


} // anonymous namespace

class ConditionTest
  : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(ConditionTest);

    CPPUNIT_TEST(testCompileTimeStateless);

  CPPUNIT_TEST_SUITE_END();
private:

  void testCompileTimeStateless()
  {
    // Tests compile-time chaining of stateless conditions.
    namespace c = meta::condition;

    // Simple conditions.
    CPPUNIT_ASSERT_EQUAL(true, (c::_or<_true, _false>::check()));
    CPPUNIT_ASSERT_EQUAL(true, (c::_or<_false, _true>::check()));

    CPPUNIT_ASSERT_EQUAL(false, (c::_and<_true, _false>::check()));
    CPPUNIT_ASSERT_EQUAL(false, (c::_and<_false, _true>::check()));

    CPPUNIT_ASSERT_EQUAL(true, (c::_and<_true, _true>::check()));

    // More than two simple conditions
    CPPUNIT_ASSERT_EQUAL(true, (c::_or<_true, _false, _true>::check()));
    CPPUNIT_ASSERT_EQUAL(true, (c::_or<_false, _true, _false>::check()));

    CPPUNIT_ASSERT_EQUAL(false, (c::_and<_true, _false, _true>::check()));
    CPPUNIT_ASSERT_EQUAL(false, (c::_and<_false, _true, _false>::check()));

    CPPUNIT_ASSERT_EQUAL(true, (c::_and<_true, _true, _true>::check()));

    // Parametrized conditions
    CPPUNIT_ASSERT_EQUAL(true, (c::_or<less, more>::check(42, 666)));
    CPPUNIT_ASSERT_EQUAL(true, (c::_or<less, more>::check(666, 42)));
    CPPUNIT_ASSERT_EQUAL(true, (c::_or<more, less>::check(42, 666)));
    CPPUNIT_ASSERT_EQUAL(true, (c::_or<more, less>::check(666, 42)));

    CPPUNIT_ASSERT_EQUAL(false, (c::_and<less, more>::check(42, 666)));
    CPPUNIT_ASSERT_EQUAL(false, (c::_and<less, more>::check(666, 42)));
    CPPUNIT_ASSERT_EQUAL(false, (c::_and<more, less>::check(42, 666)));
    CPPUNIT_ASSERT_EQUAL(false, (c::_and<more, less>::check(666, 42)));

    CPPUNIT_ASSERT_EQUAL(true, (c::_and<more, more>::check(666, 42)));
    CPPUNIT_ASSERT_EQUAL(true, (c::_and<less, less>::check(42, 666)));

    // Will not build; parameter list mismatch.
    // CPPUNIT_ASSERT_EQUAL(true, (c::_and<less, _true>::check(42, 666)));

    // Will build, just ignores parameters.
    CPPUNIT_ASSERT_EQUAL(true, (c::_and<less, _true_ignore_params>::check(42, 666)));

    // Complex example: 42 > 666 && (false || 42 < 666)
    CPPUNIT_ASSERT_EQUAL(false, (c::_and<
          more,
          c::_or<
            _false_ignore_params,
            less
          >>::check(42, 666)));
  }
};


CPPUNIT_TEST_SUITE_REGISTRATION(ConditionTest);
