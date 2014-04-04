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
#ifndef META_COMPARISON_H
#define META_COMPARISON_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

namespace meta {

/**
 * This file defines constructs for comparing integers at compile-time. Usage
 * of all these constructs follows the same basic pattern, e.g.:
 *
 *    bool b = compare_construct<42, 666>::value;
 *
 * Depending on the comparison performed by the construct, the resulting value
 * evaluates either to true or to false.
 **/
template <typename intT, intT I1, intT I2>
struct equal
{
  enum {
    value = (I1 == I2),
  };
};



template <typename intT, intT I1, intT I2>
struct not_equal
{
  enum {
    value = (I1 != I2),
  };
};



template <typename intT, intT I1, intT I2>
struct greater
{
  enum {
    value = (I1 > I2),
  };
};



template <typename intT, intT I1, intT I2>
struct less
{
  enum {
    value = (I1 < I2),
  };
};



template <typename intT, intT I1, intT I2>
struct greater_equal
{
  enum {
    value = (I1 >= I2),
  };
};



template <typename intT, intT I1, intT I2>
struct less_equal
{
  enum {
    value = (I1 <= I2),
  };
};

/**
 * Comparison of types
 **/
namespace detail {

template <
  typename T1,
  typename T2,
  bool PICK_FIRST
>
struct compare_types_helper
{
  typedef T2 type;
};

template <
  typename T1,
  typename T2
>
struct compare_types_helper<T1, T2, true>
{
  typedef T1 type;
};


template <
  typename T1,
  typename T2,
  template <typename, size_t, size_t> class comparatorT
>
struct compare_types
{
  enum {
    value = comparatorT<size_t, sizeof(T1), sizeof(T2)>::value,
  };

  typedef typename compare_types_helper<
    T1,
    T2,
    bool(value)
  >::type type;
};

} // namespace detail

template <typename T1, typename T2>
struct size_equal : public detail::compare_types<T1, T2, equal> {};

template <typename T1, typename T2>
struct size_not_equal : public detail::compare_types<T1, T2, not_equal> {};

template <typename T1, typename T2>
struct size_greater : public detail::compare_types<T1, T2, greater> {};

template <typename T1, typename T2>
struct size_less : public detail::compare_types<T1, T2, less> {};

template <typename T1, typename T2>
struct size_greater_equal : public detail::compare_types<T1, T2, greater_equal> {};

template <typename T1, typename T2>
struct size_less_equal : public detail::compare_types<T1, T2, less_equal> {};


} // namespace meta


#endif // guard
