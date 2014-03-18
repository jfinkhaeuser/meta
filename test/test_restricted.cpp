/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.co.uk>
 *
 * Copyright (c) 2014 Unwesen Ltd.
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

#if defined(HAVE_BOOST_DATE_TIME_POSIX_TIME_POSIX_TIME_HPP)
#include <boost/date_time/posix_time/posix_time.hpp>
#endif // HAVE_BOOST_POSIXTIME


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

#if defined(HAVE_BOOST_DATE_TIME_POSIX_TIME_POSIX_TIME_HPP)
    CPPUNIT_TEST(testRestrictedSpeed);
#endif // HAVE_BOOST_POSIXTIME

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



#if defined(HAVE_BOOST_DATE_TIME_POSIX_TIME_POSIX_TIME_HPP)
  void testRestrictedSpeed()
  {
    // Test the overhead of a restricted int without restrictions. People
    // sometimes ask whether or not the overhead is worth adding such checks.

    namespace bpt = boost::posix_time;

    typedef meta::restricted<int> no_restrict_int;

    // Takes ~0.1 sec on my machine
    const int amount = 10000000;

    // First, fill a vector of N regular ints.
    std::vector<int> vec1;
    vec1.reserve(amount);

    bpt::ptime plain_start = bpt::microsec_clock::universal_time();
    for (int i = 1 ; i <= amount ; ++i) {
      vec1.push_back(i);
    }
    bpt::ptime plain_end = bpt::microsec_clock::universal_time();

    // Then, do the same with a restricted int (with no restrictions, i.e.
    // no_restrict_int)
    std::vector<no_restrict_int> vec2;
    vec2.reserve(amount);

    bpt::ptime no_restrict_start = bpt::microsec_clock::universal_time();
    for (int i = 1 ; i <= amount ; ++i) {
      vec2.push_back(i);
    }
    bpt::ptime no_restrict_end = bpt::microsec_clock::universal_time();


    bpt::time_duration plain_time = plain_end - plain_start;
    bpt::time_duration no_restrict_time = no_restrict_end - no_restrict_start;

    // Because wall time sucks as a performance/overhead measurement, we'll
    // just make the simplifying assumption that a 20% difference between
    // both durations is a failure - there should be close to no overhead
    // after all.
    const double percentage = 0.2;

    bpt::time_duration::tick_type ms = std::max(plain_time, no_restrict_time)
      .total_microseconds();
    bpt::time_duration::tick_type diff_ms = std::labs(
        (plain_time - no_restrict_time).total_microseconds());
    CPPUNIT_ASSERT_MESSAGE("This will likely fail if compiled with -O0.",
        diff_ms < (ms * percentage));

    // Just for the hell of it, add another test where the int is restricted
    // to be positive.

    typedef meta::restricted<int,
            meta::restrictions::numeric::positive> restrict_int;

    CPPUNIT_ASSERT_THROW(restrict_int(-1),
        meta::restrictions::violation_error);

    std::vector<restrict_int> vec3;
    vec3.reserve(amount);

    bpt::ptime restrict_start = bpt::microsec_clock::universal_time();
    for (int i = 1 ; i <= amount ; ++i) {
      vec3.push_back(i);
    }
    bpt::ptime restrict_end = bpt::microsec_clock::universal_time();

    bpt::time_duration restrict_time = restrict_end - restrict_start;

    // These checks can't be expensive.
    ms = std::max(plain_time, restrict_time).total_microseconds();
    diff_ms = std::labs((plain_time - restrict_time).total_microseconds());
    CPPUNIT_ASSERT(diff_ms < (ms * percentage));
  }
#endif // HAVE_BOOST_POSIXTIME



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
