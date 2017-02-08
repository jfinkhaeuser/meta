/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
 *
 * Copyright (c) 2014-2015 Unwesen Ltd.
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

#ifndef META_MATH_H
#define META_MATH_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>
#include <meta/inttypes.h>

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
  enum META_ENUM_CLASS(GCD, intT) {
    result = gcd<intT, B, A % B> META_ENUM_CLASS_NS(GCD) ::result,
  };
};

// Need a bunch of specializations because non-type arguments many not depend
// on a type argument.
#define META_MATH_GCD_SPECIALIZE(type)  \
  template <type A>                     \
  struct gcd<type, A, type(0)>          \
  {                                     \
    enum META_ENUM_CLASS(GCD, type) {   \
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
  enum META_ENUM_CLASS(RATIO, intT) {
    GCD = gcd<intT, _DIVIDEND, _DIVISOR> META_ENUM_CLASS_NS(GCD) ::result,
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
    static_cast<typename ratioT::int_t>(ratioT META_ENUM_CLASS_NS(RATIO) ::DIVISOR),
    static_cast<typename ratioT::int_t>(ratioT META_ENUM_CLASS_NS(RATIO) ::DIVIDEND)
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
  enum META_ENUM_CLASS(HELPER, intT) {
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
  enum META_ENUM_CLASS(HELPER, intT) {
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
    static_cast<int_t>(ratioA META_ENUM_CLASS_NS(RATIO) ::DIVIDEND),
    static_cast<int_t>(ratioA META_ENUM_CLASS_NS(RATIO) ::DIVISOR),
    static_cast<int_t>(ratioB META_ENUM_CLASS_NS(RATIO) ::DIVIDEND),
    static_cast<int_t>(ratioB META_ENUM_CLASS_NS(RATIO) ::DIVISOR)
  > helper_t;

public:
  enum META_ENUM_CLASS(RATIO, int_t) {
    GCD = gcd<
      int_t,
      static_cast<int_t>(helper_t META_ENUM_CLASS_NS(HELPER) ::TMP1),
      static_cast<int_t>(helper_t META_ENUM_CLASS_NS(HELPER) ::TMP2)
    > META_ENUM_CLASS_NS(GCD) ::result,
    DIVIDEND = static_cast<int_t>(helper_t META_ENUM_CLASS_NS(HELPER) ::TMP1) / GCD,
    DIVISOR = static_cast<int_t>(helper_t META_ENUM_CLASS_NS(HELPER) ::TMP2) / GCD,
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
