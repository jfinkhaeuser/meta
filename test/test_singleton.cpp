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

#include <meta/singleton.h>

namespace {

// Destruction policy used to manually destroy instances
struct manual_destruction_policy
{
  typedef void (*func_t)(void);

  static func_t func;

  static void register_destruction_callback(func_t _func)
  {
    func = _func;
  }


  static void destroy_now()
  {
    func();
  }
};

manual_destruction_policy::func_t
manual_destruction_policy::func = nullptr;



// Singleton for the tests. We'll template it, because that allows us to
// re-use it in several tests.
template <
  typename T,
  typename policyT = meta::detail::default_destruction_policy
>
struct MySingleton
  : public meta::singleton<
      MySingleton<T, policyT>,
      meta::detail::default_lock_policy,
      policyT
    >
{
  T member;

  static bool destroyed;

  MySingleton(T const & _member = 42)
    : meta::singleton<
        MySingleton<T, policyT>,
        meta::detail::default_lock_policy,
        policyT
      >()
    , member(_member)
  {
    destroyed = false;
  }

  ~MySingleton()
  {
    destroyed = true;
  }
};


template <typename T, typename policyT>
bool
MySingleton<T, policyT>::destroyed = false;


} // anonymous namespace

class SingletonTest
  : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(SingletonTest);

    CPPUNIT_TEST(testSingletonDefaultCtor);
    CPPUNIT_TEST(testSingletonCtorParameters);
    CPPUNIT_TEST(testDestruction);

  CPPUNIT_TEST_SUITE_END();
private:

  void testSingletonDefaultCtor()
  {
    CPPUNIT_ASSERT_EQUAL(false, MySingleton<int>::destroyed);

    MySingleton<int> & s = MySingleton<int>::mutable_instance();
    CPPUNIT_ASSERT_EQUAL(int(42), s.member);

    s.member = 666;
    MySingleton<int> const & s2 = MySingleton<int>::const_instance();
    CPPUNIT_ASSERT_EQUAL(int(666), s2.member);
    CPPUNIT_ASSERT_EQUAL(s.member, s2.member);
    CPPUNIT_ASSERT_EQUAL(&s.member, const_cast<int*>(&s2.member));

    // Lives until end of program
    CPPUNIT_ASSERT_EQUAL(false, MySingleton<int>::destroyed);
  }



  void testSingletonCtorParameters()
  {
    CPPUNIT_ASSERT_EQUAL(false, MySingleton<long>::destroyed);

    MySingleton<long> & s = MySingleton<long>::mutable_instance(1234);
    CPPUNIT_ASSERT_EQUAL(long(1234), s.member);

    s.member = 666;
    // Even though we pass parameters to the second instance, they're ignored
    // because the object already exists.
    MySingleton<long> const & s2 = MySingleton<long>::const_instance(1234);
    CPPUNIT_ASSERT_EQUAL(long(666), s2.member);
    CPPUNIT_ASSERT_EQUAL(s.member, s2.member);
    CPPUNIT_ASSERT_EQUAL(&s.member, const_cast<long*>(&s2.member));

    // Lives until end of program
    CPPUNIT_ASSERT_EQUAL(false, MySingleton<long>::destroyed);
  }



  void testDestruction()
  {
    CPPUNIT_ASSERT_EQUAL(false, (MySingleton<float, manual_destruction_policy>::destroyed));

    MySingleton<float, manual_destruction_policy> const & s =
      MySingleton<float, manual_destruction_policy>::const_instance(3.14);
    CPPUNIT_ASSERT_EQUAL(float(3.14), s.member);

    CPPUNIT_ASSERT_EQUAL(false, (MySingleton<float, manual_destruction_policy>::destroyed));
    manual_destruction_policy::destroy_now();
    CPPUNIT_ASSERT_EQUAL(true, (MySingleton<float, manual_destruction_policy>::destroyed));

    // Since the instance is detroyed, we can now re-create it, I hope!
    MySingleton<float, manual_destruction_policy> const & s2 =
      MySingleton<float, manual_destruction_policy>::const_instance(2.78);
    CPPUNIT_ASSERT_EQUAL(false, (MySingleton<float, manual_destruction_policy>::destroyed));
    CPPUNIT_ASSERT_EQUAL(float(2.78), s2.member);

    manual_destruction_policy::destroy_now();
    CPPUNIT_ASSERT_EQUAL(true, (MySingleton<float, manual_destruction_policy>::destroyed));
  }
};


CPPUNIT_TEST_SUITE_REGISTRATION(SingletonTest);
