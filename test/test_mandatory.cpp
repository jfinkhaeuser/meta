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

#include <cstdlib>

#include <vector>

#include <cppunit/extensions/HelperMacros.h>

#include <meta/mandatory.h>

namespace {

meta::throw_if_unchecked<int> mandatory_function()
{
    return 42;
}

} // anonymous namespace

class MandatoryTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(MandatoryTest);

        CPPUNIT_TEST(testMandatory);

    CPPUNIT_TEST_SUITE_END();
private:

    void testMandatory()
    {
        int ret = 0;
        CPPUNIT_ASSERT_THROW(mandatory_function(), std::logic_error);
        CPPUNIT_ASSERT_NO_THROW(static_cast<meta::ignore_return_value>(mandatory_function()));
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
        CPPUNIT_ASSERT_NO_THROW(meta::ignore_return_value x = mandatory_function());
#pragma GCC diagnostic pop
        CPPUNIT_ASSERT_NO_THROW(ret = mandatory_function());

        ret = mandatory_function();
        CPPUNIT_ASSERT_EQUAL(ret, 42);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(MandatoryTest);
