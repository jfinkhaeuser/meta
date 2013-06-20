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

#include <meta/byteorder.h>


class ByteOrderTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(ByteOrderTest);

        CPPUNIT_TEST(testByteOrder);

    CPPUNIT_TEST_SUITE_END();
private:

    void testByteOrder()
    {
        namespace b = meta::byte_order;

        uint16_t x = 1234;
        CPPUNIT_ASSERT_EQUAL(x, b::convert<>::ntoh(b::convert<>::hton(x)));
        uint32_t y = 1234;
        CPPUNIT_ASSERT_EQUAL(y, b::convert<>::ntoh(b::convert<>::hton(y)));
        uint64_t z = 1234;
        CPPUNIT_ASSERT_EQUAL(z, b::convert<>::ntoh(b::convert<>::hton(z)));

        if (b::host_byte_order() == b::META_BIG_ENDIAN) {
            CPPUNIT_ASSERT_EQUAL(static_cast<uint16_t>(1234), b::to_host(x, b::META_BIG_ENDIAN));
            CPPUNIT_ASSERT_EQUAL(static_cast<uint16_t>(53764), b::to_host(x, b::META_LITTLE_ENDIAN));
        } else {
            CPPUNIT_ASSERT_EQUAL(static_cast<uint16_t>(53764), b::to_host(x, b::META_BIG_ENDIAN));
            CPPUNIT_ASSERT_EQUAL(static_cast<uint16_t>(1234), b::to_host(x, b::META_LITTLE_ENDIAN));
        }
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(ByteOrderTest);
