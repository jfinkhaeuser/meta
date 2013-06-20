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

#include <cppunit/extensions/HelperMacros.h>

#include <meta/pointers.h>

namespace {

struct pointer_test_struct
{
    pointer_test_struct()
        : m_counter(0)
    {
    }

    void inc() const
    {
        ++m_counter;
    }

    mutable int m_counter;
};

template <template <typename> class pointerT>
void pointer_tester(typename pointerT<pointer_test_struct>::type param)
{
    param->inc();
}


struct copy_policy_test_struct
{
    copy_policy_test_struct()
        : m_copied(false)
    {
    }

    copy_policy_test_struct(copy_policy_test_struct const & other)
        : m_copied(true)
    {
    }

    bool m_copied;
};

} // anonymous namespace

class PointerPolicyTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(PointerPolicyTest);

        CPPUNIT_TEST(testPointerTypes);
        CPPUNIT_TEST(testShallowCopyPolicy);
        CPPUNIT_TEST(testDeepCopyPolicy);

    CPPUNIT_TEST_SUITE_END();
private:

    void testPointerTypes()
    {
        // test that pointer_tester compiles and works on all types of pointers...
        pointer_test_struct x;

        pointer_tester<meta::pointers::pointer>(&x);
        CPPUNIT_ASSERT_EQUAL(x.m_counter, 1);

        pointer_tester<meta::pointers::const_pointer>(&x);
        CPPUNIT_ASSERT_EQUAL(x.m_counter, 2);

        pointer_tester<meta::pointers::const_pointer_const>(&x);
        CPPUNIT_ASSERT_EQUAL(x.m_counter, 3);
    }


    void testShallowCopyPolicy()
    {
        using namespace meta::pointers;

        copy_policy_test_struct * x = new copy_policy_test_struct();
        CPPUNIT_ASSERT_EQUAL(x->m_copied, false);

        shallow_copy<copy_policy_test_struct, pointer> first = x;
        CPPUNIT_ASSERT_EQUAL(first.m_pointer->m_copied, false);

        // after copying shallow_copy, it's embedded m_pointer must not have been
        // copy constructed.
        shallow_copy<copy_policy_test_struct, pointer> second = first;
        CPPUNIT_ASSERT_EQUAL(second.m_pointer->m_copied, false);

        delete x;
    }


    void testDeepCopyPolicy()
    {
        using namespace meta::pointers;

        copy_policy_test_struct * x = new copy_policy_test_struct();
        CPPUNIT_ASSERT_EQUAL(x->m_copied, false);

        deep_copy<copy_policy_test_struct, pointer> first = x;
        CPPUNIT_ASSERT_EQUAL(first.m_pointer->m_copied, false);

        // after copying deep_copy, it's embedded m_pointer must have been
        // copy constructed.
        deep_copy<copy_policy_test_struct, pointer> second = first;
        CPPUNIT_ASSERT_EQUAL(second.m_pointer->m_copied, true);

        delete x;
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(PointerPolicyTest);
