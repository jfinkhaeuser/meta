/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.co.uk>
 *
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

#ifndef META_DETAIL_CHAIN_HELPER_TCC
#define META_DETAIL_CHAIN_HELPER_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

namespace meta {
namespace condition {
namespace detail {


/**
 * Generic definition of chain_helper. Takes a dual operator
 * (see dual_operators.tcc) as it's first argument and a list of conditions
 * as the remaining.
 *
 * This will get used if you provide no conditions at all, and won't compile
 * if you try to call check() - that's by design. The check() function makes
 * no sense when there are no conditions provided.
 **/
template <
  template <typename, typename> class Operator,
  typename... Conditions
>
struct chain_helper;


/**
 * Specialization for a single element. It ignores the operator, and just
 * calls check() on the single element.
 **/
template <
  template <typename, typename> class Operator,
  typename Head
>
struct chain_helper<Operator, Head>
{
  template <typename ...Args>
  static inline bool check(Args... args)
  {
    return Head::check(args...);
  }
};



/**
 * Specialization for >1 elements. Splits the elements into a head and rest,
 * and delegates to the other specializations accordingly. This actually uses
 * the dual operator recursively to construct the chain.
 **/
template <
  template <typename, typename> class Operator,
  typename Head,
  typename... ConditionsRest>
struct chain_helper<Operator, Head, ConditionsRest...>
{
  template <typename ...Args>
  static inline bool check(Args... args)
  {
    return Operator<
      Head,
      chain_helper<Operator, ConditionsRest...>
    >::check(args...);
  }
};


}}} // namespace meta::condition::detail

#endif // guard
