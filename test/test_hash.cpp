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

#include <string>

#include <meta/hash.h>


class HashTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(HashTest);

      CPPUNIT_TEST(testHashCombine);

    CPPUNIT_TEST_SUITE_END();

private:

    void testHashCombine()
    {
      namespace h = meta::hash;

      size_t h0 = h::multi_hash(123);

      int a = 42;
      std::string b = "hello, world";

      size_t h1 = h::multi_hash(a, b);
      CPPUNIT_ASSERT(h0 != h1);

      a = 666;
      size_t h2 = h::multi_hash(a, b);
      CPPUNIT_ASSERT(h1 != h2);

      a = 42;
      size_t h3 = h::multi_hash(a, b);
      CPPUNIT_ASSERT_EQUAL(h1, h3);

      size_t h4 = h::multi_hash(a, b, 123);
      CPPUNIT_ASSERT(h1 != h4);
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(HashTest);
