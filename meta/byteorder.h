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
#ifndef META_BYTEORDER_H
#define META_BYTEORDER_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

#if defined(STDCXX_0X_HEADERS) || defined(STDCXX_98_HEADERS)
#  include <cstdint>
#  include <cinttypes>
#elif defined(STDCXX_TR1_HEADERS)
#  include <tr1/cstdint>
#  include <tr1/cinttypes>
#else
#  include <stdint.h>
#  include <inttypes.h>
#endif

#if defined(HAVE_ENDIAN_H)
#  include <endian.h>
#endif

#if defined(HAVE_BYTESWAP_H)
#  include <byteswap.h>
#endif


/**
 * If we can grab the byte order from the compiler, do that.
 **/
#if defined(__GNUC__)
   // We have GCC, which means we get our endianness via autoconf's
   // WORDS_BIGENDIAN define.
#  if defined(WORDS_BIGENDIAN)
#    define META_BYTE_ORDER_TMP 4321
#  else // WORDS_BIGENDIAN
#    define META_BYTE_ORDER_TMP 1234
#  endif // WORDS_BIGENDIAN
#elif defined(HAVE_ENDIAN_H)
#  define META_BYTE_ORDER_TMP __BYTE_ORDER
#else
   // TODO support other compilers
#  error "Could not determine byte order either from the compiler or <endian.h>."
#endif // __GNUC__


namespace meta {
namespace byte_order {

/**
 * Definition of META_BIG_ENDIAN and META_LITTLE_ENDIAN enum values.
 **/

enum endian
{
  META_UNKNOWN_ENDIAN = -1,
  META_BIG_ENDIAN     = 0,
  META_LITTLE_ENDIAN  = 1
};


/**
 * Used to define META_BYTE_ORDER, see below.
 **/
namespace detail {

template <int ENDIAN>
struct host_helper {
  enum { value = META_UNKNOWN_ENDIAN };
};

template <>
struct host_helper<4321>
{
  enum { value = META_BIG_ENDIAN };
};

template <>
struct host_helper<1234>
{
  enum { value = META_LITTLE_ENDIAN };
};

} // namespace detail


/**
 * In the end, host_byte_order does not add anything to META_BYTE_ORDER_TMP -
 * except that it's either -1, 0 or 1, i.e. values that are more easily used
 * as array indices (see decision matrix in to_host() below).
 *
 * If you want to avoid warnings and manual casts, compare against the
 * return value of host_byte_order() below instead of META_BYTE_ORDER
 * directly.
 **/
enum { META_BYTE_ORDER = detail::host_helper<META_BYTE_ORDER_TMP>::value };

#if defined(META_BYTE_ORDER_TMP)
#  undef META_BYTE_ORDER_TMP
#endif


/**
 * Simple function returning the value of META_BYTE_ORDER cast to endian,
 * to be able to compare it against endian values without warnings (or
 * manually casting yourself).
 **/
inline endian host_byte_order()
{
  return static_cast<endian>(META_BYTE_ORDER);
}


/**
 * Simple function to return a string representation of the passed endian value,
 * that is "LE", "BE" or "--" for unknown endianness.
 **/
inline char const * to_string(int source_endian)
{
  return (source_endian == META_UNKNOWN_ENDIAN
          ? "--"
          : (source_endian == META_BIG_ENDIAN
              ? "BE"
              : "LE"
            )
          );
}


/**
 * Simple function to return the opposite of the byte order passed to it,
 * i.e. returns META_LITTLE_ENDIAN if META_BIG_ENDIAN is passed and
 * vice versa.
 *
 * If META_UNKNOWN_ENDIAN is passed, the same is returned.
 **/
inline endian opposite(int arg)
{
  return (arg == META_UNKNOWN_ENDIAN
          ? META_UNKNOWN_ENDIAN
          : (arg == META_BIG_ENDIAN
              ? META_LITTLE_ENDIAN
              : META_BIG_ENDIAN
            )
          );
}


/**
 * Swap byte order of various integer sizes.
 **/
inline uint16_t swap(uint16_t const & orig)
{
#if defined(HAVE_BYTESWAP_H)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wold-style-cast"
  return bswap_16(orig);
#  pragma GCC diagnostic pop
#else
  return ((orig & 0xff00) >> 8) |
         ((orig & 0x00ff) << 8);
#endif
}

inline int16_t swap(int16_t const & orig)
{
#if defined(HAVE_BYTESWAP_H)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wold-style-cast"
  return bswap_16(orig);
#  pragma GCC diagnostic pop
#else
  return swap(static_cast<uint16_t>(orig));
#endif
}

inline uint32_t swap(uint32_t const & orig)
{
#if defined(HAVE_BYTESWAP_H)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wold-style-cast"
  return bswap_32(orig);
#  pragma GCC diagnostic pop
#else
  return ((orig & 0xff000000UL) >> 24) |
         ((orig & 0x00ff0000UL) >> 8)  |
         ((orig & 0x0000ff00UL) << 8)  |
         ((orig & 0x000000ffUL) << 24);
#endif
}

inline int32_t swap(int32_t const & orig)
{
#if defined(HAVE_BYTESWAP_H)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wold-style-cast"
  return bswap_32(orig);
#  pragma GCC diagnostic pop
#else
  return swap(static_cast<uint32_t>(orig));
#endif
}

inline uint64_t swap(uint64_t const & orig)
{
#if defined(HAVE_BYTESWAP_H)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wold-style-cast"
  return bswap_64(orig);
#  pragma GCC diagnostic pop
#else
  return ((orig & (static_cast<uint64_t>(0xff) << 56)) >> 56) |
         ((orig & (static_cast<uint64_t>(0xff) << 48)) >> 40) |
         ((orig & (static_cast<uint64_t>(0xff) << 40)) >> 24) |
         ((orig & (static_cast<uint64_t>(0xff) << 32)) >> 8)  |
         ((orig & (static_cast<uint64_t>(0xff) << 24)) << 8)  |
         ((orig & (static_cast<uint64_t>(0xff) << 16)) << 24) |
         ((orig & (static_cast<uint64_t>(0xff) <<  8)) << 40) |
         ((orig &  static_cast<uint64_t>(0xff)       ) << 56);
#endif
}

inline int64_t swap(int64_t const & orig)
{
#if defined(HAVE_BYTESWAP_H)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wold-style-cast"
  return bswap_64(orig);
#  pragma GCC diagnostic pop
#else
  return swap(static_cast<uint64_t>(orig));
#endif
}


/**
 * Unspecialized declaration of convert. Specialized versions of convert contain
 * hton() and ntoh() functions, which accept any integer type whatsoever. That
 * makes them easier to use in heavily templated code, where the type of integer
 * is not necessarily known. The definitions above are also platform independent
 * (with the drawback that platform dependent specializations /may/ exist and be
 * faster - though that's fairly unlikely).
 **/
template <int ENDIAN = META_BYTE_ORDER>
struct convert
{
};



/**
 * META_LITTLE_ENDIAN ntoh()/hton() conversions.
 **/
template <>
struct convert<META_LITTLE_ENDIAN>
{
  template <typename intT>
  inline static intT hton(intT const & orig)
  {
    // because network byte order is big endian, we need to swap the
    // endianness of the input integer.
    return swap(orig);
  }

  template <typename intT>
  inline static intT ntoh(intT const & orig)
  {
    // because network byte order is big endian, we need to swap the
    // endianness of the input integer.
    return swap(orig);
  }
};



/**
 * META_BIG_ENDIAN ntoh()/hton() conversions.
 **/
template <>
struct convert<META_BIG_ENDIAN>
{
  template <typename intT>
  inline static intT hton(intT const & orig)
  {
    // network byte order is already big endian, so no changes required.
    return orig;
  }

  template <typename intT>
  inline static intT ntoh(intT const & orig)
  {
    // network byte order is already big endian, so no changes required.
    return orig;
  }
};


/**
 * Use the to_host() function to convert an integer value from a given endianess
 * to the endianess of the host. The function detects whether endianess swapping
 * needs to be performed or not.
 **/
template <typename intT>
inline intT
to_host(intT const & int_value, endian int_endian)
{
  static bool const decision[2][2] = {
    // int is BE   int is LE
    { false,      true,  }, // host BE
    { true,       false, }, // host LE
  };

  if (decision[META_BYTE_ORDER][int_endian]) {
    return swap(int_value);
  }
  return int_value;
}


/**
 * The from_host() function more or less does the same as the to_host() function
 * above, except in reverse - it converts an integer from host byte order to the
 * specified byte order.
 **/
template <typename intT>
inline intT
from_host(intT const & int_value, endian int_endian)
{
  static bool const decision[2][2] = {
    // int is BE   int is LE
    { false,      true,  }, // host BE
    { true,       false, }, // host LE
  };

  if (decision[META_BYTE_ORDER][int_endian]) {
    return swap(int_value);
  }
  return int_value;
}

}} // namespace meta::byte_order

#endif // guard
