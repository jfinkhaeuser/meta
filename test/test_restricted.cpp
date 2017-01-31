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

#include <meta/meta-config.h>

#include <meta/restricted.h>
#include <meta/restrictions.h>

#include <test/timing.h>

#include <algorithm>

namespace {

// Shut the compiler up
template <typename T>
inline void do_something(T const &)
{
};


/** test a few restrictions, thereby proving the general mechanism */
meta::restricted<int, meta::restrictions::numeric::even>
restricted_function(meta::restricted<
        std::string,
        meta::restrictions::container::non_empty
    >, int return_value)
{
    return return_value;
}

} // anonymous namespace

class RestrictedTest
  : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(RestrictedTest);

    CPPUNIT_TEST(testRestricted);
    CPPUNIT_TEST(testRestrictedComplex);
    CPPUNIT_TEST(testRestrictedSpeed);

  CPPUNIT_TEST_SUITE_END();
private:

  void testRestricted()
  {
    // doesn't throw, all restrictions are fulfilled
    CPPUNIT_ASSERT_NO_THROW(restricted_function(std::string("Hello, world!"), 10));

    // throws because std::string() produces a non-empty container
    CPPUNIT_ASSERT_THROW(restricted_function(std::string(), 10), meta::restrictions::violation_error);

    // throws because 11 is not an even integer
    CPPUNIT_ASSERT_THROW(restricted_function(std::string("Hello, world!"), 11), meta::restrictions::violation_error);

    // test a special numeric restriction, i.e. that a pointer is non-zero:
    typedef meta::restricted<
        char const *,
        meta::restrictions::numeric::non_default_value
    > non_zero_char_pointer;
    CPPUNIT_ASSERT_THROW(non_zero_char_pointer p, meta::restrictions::violation_error);
    CPPUNIT_ASSERT_NO_THROW(non_zero_char_pointer p = "Hello, world!"; do_something(p));

    // Swap two values
    typedef meta::restricted<int, meta::restrictions::numeric::even> even_int_t;
    even_int_t a = 2;
    CPPUNIT_ASSERT_THROW(a = 3, meta::restrictions::violation_error);
    even_int_t b = 4;
    CPPUNIT_ASSERT_THROW(b = 3, meta::restrictions::violation_error);
    CPPUNIT_ASSERT_NO_THROW(a.swap(b));
    CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(b));
    CPPUNIT_ASSERT_EQUAL(4, static_cast<int>(a));
    CPPUNIT_ASSERT_NO_THROW(std::swap(a, b));
    CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(a));
    CPPUNIT_ASSERT_EQUAL(4, static_cast<int>(b));

    // Silly thing, really... test that assigning to even_int_t returns a value.
    CPPUNIT_ASSERT(a = 6);
  }



  void testRestrictedSpeed()
  {
    // Test the overhead of a restricted int without restrictions. People
    // sometimes ask whether or not the overhead is worth adding such checks.
    namespace tt = test::timing;

    typedef meta::restricted<int> no_restrict_int;

    // Takes ~0.1 sec on my machine
    const int amount = 10000000;

    // First, fill a vector of N regular ints.
    std::vector<int> vec1;
    vec1.reserve(amount);

    tt::timer plain;
    for (int i = 1 ; i <= amount ; ++i) {
      vec1.push_back(i);
    }
    int64_t plain_time = plain.duration();

    // Then, do the same with a restricted int (with no restrictions, i.e.
    // no_restrict_int)
    std::vector<no_restrict_int> vec2;
    vec2.reserve(amount);

    tt::timer no_restrict;
    for (int i = 1 ; i <= amount ; ++i) {
      vec2.push_back(i);
    }
    int64_t no_restrict_time = no_restrict.duration();

    // Because wall time sucks as a performance/overhead measurement, we'll
    // just make the simplifying assumption that a 200% difference between
    // both durations is a failure - there should be close to no overhead
    // after all.
    const double percentage = 2;

    int64_t us = std::max(plain_time, no_restrict_time);
    int64_t diff_us = std::labs(plain_time - no_restrict_time);
    CPPUNIT_ASSERT_MESSAGE("Using wall time as performance measure sucks, so this can fail. Make sure you're running an optimized build.",
        diff_us < (us * percentage));

    // Just for the hell of it, add another test where the int is restricted
    // to be positive.

    typedef meta::restricted<int,
            meta::restrictions::numeric::positive> restrict_int;

    CPPUNIT_ASSERT_THROW(restrict_int(-1),
        meta::restrictions::violation_error);

    std::vector<restrict_int> vec3;
    vec3.reserve(amount);

    tt::timer restricted;
    for (int i = 1 ; i <= amount ; ++i) {
      vec3.push_back(i);
    }
    int64_t restrict_time = restricted.duration();

    // These checks can't be expensive.
    us = std::max(plain_time, restrict_time);
    diff_us = std::labs(plain_time - restrict_time);
    CPPUNIT_ASSERT(diff_us < (us * percentage));
  }



  void testRestrictedComplex()
  {
    // Test whether OR-chaining of restrictions works as intended.
    typedef meta::restricted<int,
            meta::restrictions::numeric::null_or_positive> restrict_int;

    CPPUNIT_ASSERT_THROW(restrict_int(-1),
        meta::restrictions::violation_error);

    CPPUNIT_ASSERT_NO_THROW(restrict_int(0));
    CPPUNIT_ASSERT_NO_THROW(restrict_int(1));
  }
};


CPPUNIT_TEST_SUITE_REGISTRATION(RestrictedTest);
