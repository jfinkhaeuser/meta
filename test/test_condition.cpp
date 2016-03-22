/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.de>
 *
 * Copyright (c) 2013-2015 Unwesen Ltd.
 * Copyright (c) 2016 Jens Finkhaeuser
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
  : public meta::condition::dynamic<less>
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

  bool operator()(int) const
  {
    return false;
  }
};


// Stateful condition
struct less_than_state
{
  int state;

  less_than_state(int s)
    : state(s)
  {
  }

  bool operator()(int value) const
  {
    //std::cout << "LT: comparing state " << state << " to value " << value << std::endl;
    return (value < state);
  }
};

struct more_than_state
{
  int state;

  more_than_state(int s)
    : state(s)
  {
  }

  bool operator()(int value) const
  {
    //std::cout << "MT: comparing state " << state << " to value " << value << std::endl;
    return (value > state);
  }
};

} // anonymous namespace

class ConditionTest
  : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(ConditionTest);

    CPPUNIT_TEST(testCompileTimeStateless);
    CPPUNIT_TEST(testCompileTimeStateful);

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



  void testCompileTimeStateful()
  {
    namespace c = meta::condition;

    // Stateful condition on it's own.
    less_than_state cond(666);
    CPPUNIT_ASSERT_EQUAL(true, cond(42));
    CPPUNIT_ASSERT_EQUAL(false, cond(1024));

    // Chained stateful conditions.
    c::_or<less_than_state, less_than_state> or_cond = { less_than_state(42), less_than_state(1024) };
    CPPUNIT_ASSERT_EQUAL(int(42), or_cond.item.state);
    CPPUNIT_ASSERT_EQUAL(int(1024), static_cast<meta::types::compositionlist<less_than_state>*>(&or_cond)->item.state);
    CPPUNIT_ASSERT_EQUAL(true, or_cond(7));
    CPPUNIT_ASSERT_EQUAL(true, or_cond(666));
    CPPUNIT_ASSERT_EQUAL(false, or_cond(1234));

    c::_and<less_than_state, less_than_state> and_cond = { less_than_state(42), less_than_state(1024) };
    CPPUNIT_ASSERT_EQUAL(int(42), and_cond.item.state);
    CPPUNIT_ASSERT_EQUAL(int(1024), static_cast<meta::types::compositionlist<less_than_state>*>(&and_cond)->item.state);
    CPPUNIT_ASSERT_EQUAL(true, and_cond(7));
    CPPUNIT_ASSERT_EQUAL(false, and_cond(666));
    CPPUNIT_ASSERT_EQUAL(false, and_cond(1234));

    // Complex example: 42 > 666 && (false || 42 < 666)
    c::_and<
          more_than_state,
          c::_or<
            _false_ignore_params,
            less_than_state
          >
        > complex_cond = {
          42, // more_than_state
          {
            {},   // _false_ignore_params
            1024  // less_than_state
          }};
    CPPUNIT_ASSERT_EQUAL(false, complex_cond(7));
    CPPUNIT_ASSERT_EQUAL(true, complex_cond(666));
    CPPUNIT_ASSERT_EQUAL(false, complex_cond(1234));

    // The "less" example condition is stateless, but can be used in a dynamic
    // context. Note that you must still initialize the conditions.
    less l;
    CPPUNIT_ASSERT_EQUAL(true, l(42, 666));
    c::_or<less, less> or2_cond = {{}, {}};
    CPPUNIT_ASSERT_EQUAL(true, or2_cond(42, 666));
    CPPUNIT_ASSERT_EQUAL(false, or2_cond(42, 1));
  }

};


CPPUNIT_TEST_SUITE_REGISTRATION(ConditionTest);
