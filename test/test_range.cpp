/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
 *
 * Copyright (c) 2009-2012 Jens Finkhaeuser.
 * Copyright (c) 2013-2015 Unwesen Ltd.
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

#include <meta/range.h>

#include <vector>
#include <map>

class RangeTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(RangeTest);

      CPPUNIT_TEST(testPointerRange);
      CPPUNIT_TEST(testVectorRange);
      CPPUNIT_TEST(testMultiMapRange);

    CPPUNIT_TEST_SUITE_END();

private:

    void testPointerRange()
    {
      int container[] = {0, 1, 2, 3, 4, 5}; // 6 elements

      auto r = meta::make_range(container, container + 6);
      int c = 0;
      for (auto x : r) {
        CPPUNIT_ASSERT_EQUAL(c, x);
        ++c;
      }
      CPPUNIT_ASSERT_EQUAL(int(6), c);
    }



    void testVectorRange()
    {
      std::vector<int> container = {0, 1, 2, 3, 4, 5}; // 6 elements

      auto r = meta::make_range(container.begin(), container.end());
      int c = 0;
      for (auto x : r) {
        CPPUNIT_ASSERT_EQUAL(c, x);
        ++c;
      }
      CPPUNIT_ASSERT_EQUAL(int(6), c);
    }



    void testMultiMapRange()
    {
      std::multimap<int, int> container;
      container.insert(std::pair<int, int>(42, 0));
      container.insert(std::pair<int, int>(42, 1));
      container.insert(std::pair<int, int>(666, 123));

      auto r = meta::make_range(container.equal_range(42));
      int c = 0;
      for (auto x : r) {
        CPPUNIT_ASSERT_EQUAL(c, x.second);
        ++c;
      }
      CPPUNIT_ASSERT_EQUAL(int(2), c);
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(RangeTest);
