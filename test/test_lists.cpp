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

#include <meta/typelist.h>

namespace {


struct char_type
{
  char v;

  char_type(char _v = 0)
    : v(_v)
  {
  }
};


struct int_type
{
  int v;

  int_type(int _v = 0)
    : v(_v)
  {
  }
};


struct float_type
{
  float v;

  float_type(float _v = 0)
    : v(_v)
  {
  }
};

struct double_type
{
  double v;

  double_type(double _v = 0)
    : v(_v)
  {
  }
};

} // anonymous namespace

class ListsTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(ListsTest);

      CPPUNIT_TEST(testTypeListContains);
      CPPUNIT_TEST(testTypeListAppend);
      CPPUNIT_TEST(testTypeListPrepend);
      CPPUNIT_TEST(testTypeListRevert);
      CPPUNIT_TEST(testUniqueTypeList);

      CPPUNIT_TEST(testClassListContains);
      CPPUNIT_TEST(testClassListAppend);
      CPPUNIT_TEST(testClassListPrepend);
      CPPUNIT_TEST(testClassListRevert);
      CPPUNIT_TEST(testUniqueClassList);

    CPPUNIT_TEST_SUITE_END();

private:

    template <
      template <typename...> class ListType,
      typename IntType,
      typename FloatType,
      typename DoubleType
    >
    void testThingListContains()
    {
      namespace t = meta::types;

      CPPUNIT_ASSERT(typeid(t::true_type) == typeid(typename t::contains<IntType, ListType<IntType, DoubleType>>::type));
      CPPUNIT_ASSERT(typeid(t::true_type) == typeid(typename t::contains<IntType, ListType<DoubleType, FloatType, DoubleType, IntType>>::type));
      CPPUNIT_ASSERT(typeid(t::false_type) == typeid(typename t::contains<IntType, ListType<FloatType, DoubleType>>::type));
      CPPUNIT_ASSERT(typeid(t::false_type) == typeid(typename t::contains<IntType, ListType<>>::type));

      // Invalid use of contains; not using a typelist
      CPPUNIT_ASSERT(typeid(t::false_type) == typeid(typename t::contains<IntType, IntType>::type));
    }

    void testTypeListContains()
    {
      testThingListContains<
        meta::types::typelist,
        int, float, double>();
    }

    void testClassListContains()
    {
      testThingListContains<
        meta::types::classlist,
        int_type, float_type, double_type>();
    }




    template <
      template <typename...> class ListType,
      typename IntType,
      typename FloatType,
      typename DoubleType
    >
    void testThingListAppend()
    {
      namespace t = meta::types;

      // Append non-unique
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename t::append<FloatType, ListType<IntType>>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType, DoubleType>) == typeid(typename t::append<DoubleType, ListType<IntType, FloatType>>::type));

      CPPUNIT_ASSERT(typeid(ListType<IntType>) == typeid(typename t::append<IntType, ListType<>>::type));

      // Append - unique
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename t::append_unique<FloatType, ListType<IntType>>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename t::append_unique<IntType, ListType<IntType, FloatType>>::type));
      CPPUNIT_ASSERT(typeid(ListType<FloatType, IntType>) == typeid(typename t::append_unique<IntType, ListType<FloatType, IntType>>::type));

      CPPUNIT_ASSERT(typeid(ListType<IntType>) == typeid(typename t::append_unique<IntType, ListType<>>::type));

      // Invalid use of append_unique; will not compile
      // CPPUNIT_ASSERT(typeid(ListType<IntType, IntType>) == typeid(typename t::append_unique<IntType, IntType>::type));
    }

    void testTypeListAppend()
    {
      testThingListAppend<
        meta::types::typelist,
        int, float, double>();

      // We can also test type lists - but not class lists - with duplicate
      // entries.
      namespace t = meta::types;
      CPPUNIT_ASSERT(typeid(t::typelist<int, float, int>) == typeid(typename t::append<int, t::typelist<int, float>>::type));
      CPPUNIT_ASSERT(typeid(t::typelist<int, float, int, double>) == typeid(typename t::append<double, t::typelist<int, float, int>>::type));
    }

    void testClassListAppend()
    {
      testThingListAppend<
        meta::types::classlist,
        int_type, float_type, double_type>();
    }




    template <
      template <typename...> class ListType,
      typename IntType,
      typename FloatType,
      typename DoubleType
    >
    void testThingListPrepend()
    {
      namespace t = meta::types;

      // Prepend - non-unique
      CPPUNIT_ASSERT(typeid(ListType<FloatType, IntType>) == typeid(typename t::prepend<FloatType, ListType<IntType>>::type));
      CPPUNIT_ASSERT(typeid(ListType<DoubleType, FloatType, IntType>) == typeid(typename t::prepend<DoubleType, ListType<FloatType, IntType>>::type));

      CPPUNIT_ASSERT(typeid(ListType<IntType>) == typeid(typename t::prepend<IntType, ListType<>>::type));

      // Prepend - unique
      CPPUNIT_ASSERT(typeid(ListType<FloatType, IntType>) == typeid(typename t::prepend_unique<FloatType, ListType<IntType>>::type));
      CPPUNIT_ASSERT(typeid(ListType<FloatType, IntType>) == typeid(typename t::prepend_unique<IntType, ListType<FloatType, IntType>>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename t::prepend_unique<IntType, ListType<IntType, FloatType>>::type));

      CPPUNIT_ASSERT(typeid(ListType<IntType>) == typeid(typename t::prepend_unique<IntType, ListType<>>::type));

      // Invalid use of prepend_unique; will not compile
      // CPPUNIT_ASSERT(typeid(ListType<IntType, IntType>) == typeid(typename t::prepend_unique<IntType, IntType>::type));
    }

    void testTypeListPrepend()
    {
      testThingListPrepend<
        meta::types::typelist,
        int, float, double>();

      // We can also test type lists - but not class lists - with duplicate
      // entries.
      namespace t = meta::types;
      CPPUNIT_ASSERT(typeid(t::typelist<int, int, float>) == typeid(typename t::prepend<int, t::typelist<int, float>>::type));
      CPPUNIT_ASSERT(typeid(t::typelist<double, int, float, int>) == typeid(typename t::prepend<double, t::typelist<int, float, int>>::type));
    }

    void testClassListPrepend()
    {
      testThingListPrepend<
        meta::types::classlist,
        int_type, float_type, double_type>();
    }




    template <
      template <typename...> class ListType,
      typename CharType,
      typename IntType,
      typename FloatType,
      typename DoubleType
    >
    void testThingListRevert()
    {
      namespace t = meta::types;

      // Revert a list
      CPPUNIT_ASSERT(typeid(ListType<>) == typeid(typename t::revert<ListType<>>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType>) == typeid(typename t::revert<ListType<IntType>>::type));
      CPPUNIT_ASSERT(typeid(ListType<FloatType, IntType>) == typeid(typename t::revert<ListType<IntType, FloatType>>::type));
      CPPUNIT_ASSERT(typeid(ListType<DoubleType, FloatType, IntType>) == typeid(typename t::revert<ListType<IntType, FloatType, DoubleType>>::type));
      CPPUNIT_ASSERT(typeid(ListType<CharType, DoubleType, FloatType, IntType>) == typeid(typename t::revert<ListType<IntType, FloatType, DoubleType, CharType>>::type));
    }

    void testTypeListRevert()
    {
      testThingListRevert<
        meta::types::typelist,
        char, int, float, double>();
    }

    void testClassListRevert()
    {
      testThingListRevert<
        meta::types::classlist,
        char_type, int_type, float_type, double_type>();
    }




    template <
      template <typename...> class ListType,
      template <typename...> class UniqueListType,
      typename IntType,
      typename FloatType,
      typename DoubleType
    >
    void testUniqueThingList()
    {
      namespace t = meta::types;

      CPPUNIT_ASSERT(typeid(ListType<>) == typeid(typename UniqueListType<>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType>) == typeid(typename UniqueListType<IntType>::type));

      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename UniqueListType<IntType, FloatType>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename UniqueListType<IntType, FloatType, IntType>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename UniqueListType<IntType, FloatType, IntType, FloatType>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename UniqueListType<IntType, FloatType, IntType, FloatType, IntType>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename UniqueListType<IntType, FloatType, IntType, FloatType, IntType, FloatType>::type));


      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename UniqueListType<IntType, IntType, FloatType, FloatType, IntType>::type));
    }

    void testUniqueTypeList()
    {
      testUniqueThingList<
        meta::types::typelist,
        meta::types::unique_typelist,
        int, float, double>();
    }

    void testUniqueClassList()
    {
      testUniqueThingList<
        meta::types::classlist,
        meta::types::unique_classlist,
        int_type, float_type, double_type>();
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(ListsTest);
