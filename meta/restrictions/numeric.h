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

#ifndef META_DETAIL_NUMERIC_RESTRICTIONS_H
#define META_DETAIL_NUMERIC_RESTRICTIONS_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

namespace meta {
namespace restrictions {
namespace numeric {

/**
 * Ensures that a numeric value lies within a range of other values of the same
 * type. MIN and MAX themselves are valid values of the range.
 **/
template <
  typename valueT,
  valueT   MIN,
  valueT   MAX
>
struct range
{
  static inline bool check(valueT const & value)
  {
    return (MIN <= value && value <= MAX);
  }
};


/**
 * Ensures that a numeric value is divisible withour remainder by a specified
 * (long) integer value.
 **/
template <
  typename valueT,
  long DIVISOR
>
struct divisible
{
  static inline bool check(valueT const & value)
  {
    return !(value % DIVISOR);
  }
};

/**
 * The even policy checks that the passed value is even, i.e. divisible by 2
 * without remainder. This makes use of the divisible restriction above.
 **/
struct even
{
  template <
    typename valueT
  >
  static inline bool check(valueT const & value)
  {
    return divisible<valueT, 2>::check(value);
  }
};


/**
 * The odd policy checks that the passed value is odd, i.e. divisible by 2
 * with a remainder. This makes use of the divisible restriction above.
 **/
struct odd
{
  template <
    typename valueT
  >
  static inline bool check(valueT const & value)
  {
    return !divisible<valueT, 2>::check(value);
  }
};


/**
 * The equal policy checks that a valueT is equal to another valueT.
 **/
template <
  typename valueT,
  valueT OTHER
>
struct equal
{
  static inline bool check(valueT const & value)
  {
    return (value == OTHER);
  }
};


/**
 * The non_equal policy checks that a valueT is equal to another valueT.
 **/
template <
  typename valueT,
  valueT OTHER
>
struct non_equal
{
  static inline bool check(valueT const & value)
  {
    return (value != OTHER);
  }
};


/**
 * The default_value policy is a refinement of equal, in that it checks that a
 * value is equal to the value constructed via the type's default constructor.
 * For numeric values this is usually zero.
 **/
struct default_value
{
  template <
    typename valueT
  >
  static inline bool check(valueT const & value)
  {
    return (value == valueT());
  }
};


/**
 * The non_default_value policy is a refinement of equal, in that it checks that
 * a value is not equal to the value constructed via the type's default
 * constructor. For numeric values this is usually zero.
 *
 * You can use this restriction to check for non-zero pointers as well.
 **/
struct non_default_value
{
  template <
    typename valueT
  >
  static inline bool check(valueT const & value)
  {
    return (value != valueT());
  }
};


/**
 * Check whether the value is positive. The check relies on the assumption that
 * a default-constructed valueT is semantically equivalent to zero, which is the
 * case for all built in numeric types.
 **/
struct positive
{
  template <
    typename valueT
  >
  static inline bool check(valueT const & value)
  {
    return (value >= valueT());
  }
};


/**
 * Check whether the value is negative. The check relies on the assumption that
 * a default-constructed valueT is semantically equivalent to zero, which is the
 * case for all built in numeric types.
 **/
struct negative
{
  template <
    typename valueT
  >
  static inline bool check(valueT const & value)
  {
    return (value < valueT());
  }
};


/**
 * Combines the default_value restriction with the positive restriction such
 * that either must be satisfied to satisfy null_or_positive.
 **/
struct null_or_positive
  : public ::meta::condition::_or<
      default_value,
      positive
    >
{
};


/**
 * Combines the default_value restriction with the negative restriction such
 * that either must be satisfied to satisfy null_or_negative.
 **/
struct null_or_negative
  : public ::meta::condition::_or<
      default_value,
      negative
    >
{
};


}}} // namespace meta::restrictions::numeric

#endif // guard
