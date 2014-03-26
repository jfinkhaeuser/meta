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

#ifndef META_MATH_H
#define META_MATH_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

#include <stdint.h>

#include <meta/comparison.h>

namespace meta {
namespace math {

/**
 * Compute greatest common divisor at compile-time.
 **/
template <
  typename intT,
  intT A,
  intT B
>
struct gcd
{
  enum {
    result = gcd<intT, B, A % B>::result,
  };
};

// Need a bunch of specializations because non-type arguments many not depend
// on a type argument.
#define META_MATH_GCD_SPECIALIZE(type)  \
  template <type A>                     \
  struct gcd<type, A, type(0)>          \
  {                                     \
    enum {                              \
      result = A,                       \
    };                                  \
  };

META_MATH_GCD_SPECIALIZE(int8_t);
META_MATH_GCD_SPECIALIZE(uint8_t);
META_MATH_GCD_SPECIALIZE(int16_t);
META_MATH_GCD_SPECIALIZE(uint16_t);
META_MATH_GCD_SPECIALIZE(int32_t);
META_MATH_GCD_SPECIALIZE(uint32_t);
META_MATH_GCD_SPECIALIZE(int64_t);
META_MATH_GCD_SPECIALIZE(uint64_t);

#undef META_MATH_GCD_SPECIALIZE


/**
 * Define a ratio of two integers that is automatically cancelled down.
 **/
template <
  typename intT,
  intT _DIVIDEND,
  intT _DIVISOR = 1
>
struct ratio
{
  typedef intT int_t;
  enum {
    GCD = gcd<intT, _DIVIDEND, _DIVISOR>::result,
    DIVIDEND = _DIVIDEND / GCD,
    DIVISOR  = _DIVISOR / GCD,
  };
};


/**
 * Invert a ratio
 **/
template <
  typename ratioT
>
struct invert
{
  typedef ratio<
    typename ratioT::int_t,
    ratioT::DIVISOR,
    ratioT::DIVIDEND
  > type;
};


/**
 * Multiply/divide two ratios at compile-time. We need the helper struct to
 * avoid some compilation overflow errors.
 **/
namespace detail {


template <
  typename intT,
  intT A,
  intT B,
  intT C,
  intT D 
>
struct multiply_helper
{
  enum {
    TMP1 = A * C,
    TMP2 = B * D,
  };
};


template <
  typename intT,
  intT A,
  intT B,
  intT C,
  intT D
>
struct divide_helper
{
  enum {
    TMP1 = A * D,
    TMP2 = B * C,
  };
};



template <
  typename ratioA,
  typename ratioB,
  template <typename intT, intT, intT, intT, intT> class helperT
>
struct ratio_operator
{
public:
  // We need to pick the larger of the two ratio's int_t for the resulting
  // int_t.
  typedef typename ::meta::size_greater<
    typename ratioA::int_t,
    typename ratioB::int_t
  >::type int_t;

private:
  typedef helperT<
    int_t,
    ratioA::DIVIDEND,
    ratioA::DIVISOR,
    ratioB::DIVIDEND,
    ratioB::DIVISOR
  > helper_t;

public:
  enum {
    GCD = gcd<
      int_t,
      helper_t::TMP1,
      helper_t::TMP2
    >::result,
    DIVIDEND = helper_t::TMP1 / GCD,
    DIVISOR = helper_t::TMP2 / GCD,
  };
};


} // namespace detail


template <
  typename ratioA,
  typename ratioB
>
struct multiply_ratios
  : public detail::ratio_operator<
      ratioA,
      ratioB,
      detail::multiply_helper
    >
{
};

template <
  typename ratioA,
  typename ratioB
>
struct divide_ratios
  : public detail::ratio_operator<
      ratioA,
      ratioB,
      detail::divide_helper
    >
{
};


}} // namespace meta::math

#endif // guard
