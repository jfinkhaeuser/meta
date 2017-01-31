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

struct complex_type
{
  int a;
  double b;

  complex_type(double _b, int _a)
    : a(_a)
    , b(_b)
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

      CPPUNIT_TEST(testInheritanceListContains);
      CPPUNIT_TEST(testInheritanceListAppend);
      CPPUNIT_TEST(testInheritanceListPrepend);
      CPPUNIT_TEST(testInheritanceListRevert);
      CPPUNIT_TEST(testUniqueInheritanceList);

      CPPUNIT_TEST(testInheritanceListInitializer);

      CPPUNIT_TEST(testCompositionListContains);
      CPPUNIT_TEST(testCompositionListAppend);
      CPPUNIT_TEST(testCompositionListPrepend);
      CPPUNIT_TEST(testCompositionListRevert);
      CPPUNIT_TEST(testUniqueCompositionList);

      CPPUNIT_TEST(testCompositionListInitializer);

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

    void testInheritanceListContains()
    {
      testThingListContains<
        meta::types::inheritancelist,
        int_type, float_type, double_type>();
    }

    void testCompositionListContains()
    {
      testThingListContains<
        meta::types::compositionlist,
        int_type, float, double_type>();
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

      // We can also test type lists - but not inheritance lists - with duplicate
      // entries.
      namespace t = meta::types;
      CPPUNIT_ASSERT(typeid(t::typelist<int, float, int>) == typeid(typename t::append<int, t::typelist<int, float>>::type));
      CPPUNIT_ASSERT(typeid(t::typelist<int, float, int, double>) == typeid(typename t::append<double, t::typelist<int, float, int>>::type));
    }

    void testInheritanceListAppend()
    {
      testThingListAppend<
        meta::types::inheritancelist,
        int_type, float_type, double_type>();
    }

    void testCompositionListAppend()
    {
      testThingListAppend<
        meta::types::compositionlist,
        int, float_type, double_type>();

      // We can also test composition lists - but not inheritance lists - with duplicate
      // entries.
      namespace t = meta::types;
      CPPUNIT_ASSERT(typeid(t::compositionlist<int, float, int>) == typeid(typename t::append<int, t::compositionlist<int, float>>::type));
      CPPUNIT_ASSERT(typeid(t::compositionlist<int, float, int, double>) == typeid(typename t::append<double, t::compositionlist<int, float, int>>::type));
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

      // We can also test type lists - but not inheritance lists - with duplicate
      // entries.
      namespace t = meta::types;
      CPPUNIT_ASSERT(typeid(t::typelist<int, int, float>) == typeid(typename t::prepend<int, t::typelist<int, float>>::type));
      CPPUNIT_ASSERT(typeid(t::typelist<double, int, float, int>) == typeid(typename t::prepend<double, t::typelist<int, float, int>>::type));
    }

    void testInheritanceListPrepend()
    {
      testThingListPrepend<
        meta::types::inheritancelist,
        int_type, float_type, double_type>();
    }

    void testCompositionListPrepend()
    {
      testThingListPrepend<
        meta::types::compositionlist,
        int_type, float_type, double>();

      // We can also test composition lists - but not inheritance lists - with duplicate
      // entries.
      namespace t = meta::types;
      CPPUNIT_ASSERT(typeid(t::compositionlist<int, int, float>) == typeid(typename t::prepend<int, t::compositionlist<int, float>>::type));
      CPPUNIT_ASSERT(typeid(t::compositionlist<double, int, float, int>) == typeid(typename t::prepend<double, t::compositionlist<int, float, int>>::type));
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

    void testInheritanceListRevert()
    {
      testThingListRevert<
        meta::types::inheritancelist,
        char_type, int_type, float_type, double_type>();
    }

    void testCompositionListRevert()
    {
      testThingListRevert<
        meta::types::compositionlist,
        char, int_type, float, double_type>();
    }




    template <
      template <typename...> class ListType,
      typename IntType,
      typename FloatType,
      typename DoubleType
    >
    void testUniqueThingList()
    {
      namespace t = meta::types;

      CPPUNIT_ASSERT(typeid(ListType<>) == typeid(typename t::make_unique<ListType>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType>) == typeid(typename t::make_unique<ListType, IntType>::type));

      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename t::make_unique<ListType, IntType, FloatType>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename t::make_unique<ListType, IntType, FloatType, IntType>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename t::make_unique<ListType, IntType, FloatType, IntType, FloatType>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename t::make_unique<ListType, IntType, FloatType, IntType, FloatType, IntType>::type));
      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename t::make_unique<ListType, IntType, FloatType, IntType, FloatType, IntType, FloatType>::type));

      CPPUNIT_ASSERT(typeid(ListType<IntType, FloatType>) == typeid(typename t::make_unique<ListType, IntType, IntType, FloatType, FloatType, IntType>::type));
    }

    void testUniqueTypeList()
    {
      testUniqueThingList<
        meta::types::typelist,
        int, float, double>();
    }

    void testUniqueInheritanceList()
    {
      testUniqueThingList<
        meta::types::inheritancelist,
        int_type, float_type, double_type>();
    }

    void testUniqueCompositionList()
    {
      testUniqueThingList<
        meta::types::compositionlist,
        int_type, float_type, double_type>();
    }




    void testInheritanceListInitializer()
    {
      namespace t = meta::types;

      // No arguments
      {
        t::inheritancelist<int_type> a;
        CPPUNIT_ASSERT_EQUAL(int(0), static_cast<int_type *>(&a)->v);

        t::inheritancelist<int_type, float_type> b;
        CPPUNIT_ASSERT_EQUAL(int(0), static_cast<int_type *>(&b)->v);
        CPPUNIT_ASSERT_EQUAL(float(0), static_cast<float_type *>(&b)->v);
      }

      // Round braces
      {
        t::inheritancelist<int_type> a(42);
        CPPUNIT_ASSERT_EQUAL(int(42), static_cast<int_type *>(&a)->v);

        t::inheritancelist<int_type, float_type> b(42, 3.14f);
        CPPUNIT_ASSERT_EQUAL(int(42), static_cast<int_type *>(&b)->v);
        CPPUNIT_ASSERT_EQUAL(float(3.14), static_cast<float_type *>(&b)->v);
      }

      // Curly braces
      {
        t::inheritancelist<int_type> a = {42};
        CPPUNIT_ASSERT_EQUAL(int(42), static_cast<int_type *>(&a)->v);

        t::inheritancelist<int_type, float_type> b = {42, 3.14f};
        CPPUNIT_ASSERT_EQUAL(int(42), static_cast<int_type *>(&b)->v);
        CPPUNIT_ASSERT_EQUAL(float(3.14), static_cast<float_type *>(&b)->v);
      }

      // Complex initializers
      {
        // Omissions of braces don't work.
        // t::inheritancelist<int_type> a = 42;
        // t::inheritancelist<complex_type> a = {3.14, 42};

        t::inheritancelist<complex_type> a = {{3.14, 42}};
        CPPUNIT_ASSERT_EQUAL(int(42), static_cast<complex_type *>(&a)->a);
        CPPUNIT_ASSERT_EQUAL(double(3.14), static_cast<complex_type *>(&a)->b);

        t::inheritancelist<int_type, complex_type> b = {666, {3.14, 42}};
        CPPUNIT_ASSERT_EQUAL(int(666), static_cast<int_type *>(&b)->v);
        CPPUNIT_ASSERT_EQUAL(int(42), static_cast<complex_type *>(&b)->a);
        CPPUNIT_ASSERT_EQUAL(double(3.14), static_cast<complex_type *>(&b)->b);

        // Looks funny, but is the correct use of the round braces
        t::inheritancelist<complex_type> c({3.14, 42});
      }

    }



    void testCompositionListInitializer()
    {
      namespace t = meta::types;

      // No arguments
      {
        t::compositionlist<int_type> a;
        CPPUNIT_ASSERT_EQUAL(int(0), a.item.v);

        t::compositionlist<int> b;
        CPPUNIT_ASSERT_EQUAL(int(0), b.item);

        t::compositionlist<int_type, float, float> c;
        CPPUNIT_ASSERT_EQUAL(int(0), c.item.v);
        CPPUNIT_ASSERT_EQUAL(float(0), static_cast<t::compositionlist<float> *>(&c)->item);
        CPPUNIT_ASSERT_EQUAL(float(0), (static_cast<t::compositionlist<float, float> *>(&c)->item));
      }

      // Round braces
      {
        t::compositionlist<int_type> a(42);
        CPPUNIT_ASSERT_EQUAL(int(42), a.item.v);

        t::compositionlist<int> b(42);
        CPPUNIT_ASSERT_EQUAL(int(42), b.item);

        t::compositionlist<int_type, float, float> c(42, 3.14f, 2.71828f);
        CPPUNIT_ASSERT_EQUAL(int(42), c.item.v);
        CPPUNIT_ASSERT_EQUAL(float(2.71828), static_cast<t::compositionlist<float> *>(&c)->item);
        CPPUNIT_ASSERT_EQUAL(float(3.14), (static_cast<t::compositionlist<float, float> *>(&c)->item));
      }

      // Curly braces
      {
        t::compositionlist<int_type> a = {42};
        CPPUNIT_ASSERT_EQUAL(int(42), a.item.v);

        t::compositionlist<int> b = 42;
        CPPUNIT_ASSERT_EQUAL(int(42), b.item);

        t::compositionlist<int_type, float, float> c = {42, 3.14f, 2.71828f};
        CPPUNIT_ASSERT_EQUAL(int(42), c.item.v);
        CPPUNIT_ASSERT_EQUAL(float(3.14), (static_cast<t::compositionlist<float, float> *>(&c)->item));
        CPPUNIT_ASSERT_EQUAL(float(2.71828), static_cast<t::compositionlist<float> *>(&c)->item);
      }
    }

};


CPPUNIT_TEST_SUITE_REGISTRATION(ListsTest);
