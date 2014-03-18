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
struct static_chain_helper;


template <
  template <typename, typename> class Operator
>
struct static_chain_helper<Operator>
{
  template <typename... Args>
  static inline bool check(Args...)
  {
    // No conditions means always true
    return true;
  }
};


/**
 * Specialization for a single element. It ignores the operator, and just
 * calls check() on the single element.
 **/
template <
  template <typename, typename> class Operator,
  typename Head
>
struct static_chain_helper<Operator, Head>
{
  template <typename... Args>
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
struct static_chain_helper<Operator, Head, ConditionsRest...>
{
  template <typename... Args>
  static inline bool check(Args... args)
  {
    return Operator<
      Head,
      static_chain_helper<Operator, ConditionsRest...>
    >::check(args...);
  }
};


/**
 * For stateful/run-time/dynamic chains, we need to peel away the conditions
 * differently, and are using a different underlying operator.
 **/
template <
  template <typename, typename, typename...> class Operator,
  typename... Conditions
>
struct dynamic_chain_helper;


// End of recursion
template <
  template <typename, typename, typename...> class Operator
>
struct dynamic_chain_helper<Operator>
{
  template <typename... Args>
  inline bool check(::meta::types::compositionlist<> const &, Args && ...);
};


// Recursion
template <
  template <typename, typename, typename...> class Operator,
  typename Head,
  typename... ConditionsRest
>
struct dynamic_chain_helper<Operator, Head, ConditionsRest...>
  : public Operator<Head, dynamic_chain_helper<Operator, ConditionsRest...>, ConditionsRest...>
{
  template <typename... Args>
  inline bool operator()(Args && ... args)
  {
    return this->Operator<
      Head,
      dynamic_chain_helper<Operator, ConditionsRest...>,
      ConditionsRest...
    >::operator()(args...);
  }
};


}}} // namespace meta::condition::detail

#endif // guard
