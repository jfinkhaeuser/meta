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

#ifndef META_CONDITION_H
#define META_CONDITION_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

#include <utility>

#include <meta/typelist.h>

#include <meta/detail/dual_operators.tcc>
#include <meta/detail/chain_helper.tcc>
#include <meta/detail/dynamic.tcc>


namespace meta {
namespace condition {

/**
 * This header defines two variadic templates, _and and _or for chaining
 * together functors that represent conditions via && and || operators.
 *
 * Both constructs work whether the functors are stateful or stateless; the
 * only requirement is that all functors used in a construct are either one
 * or the other.
 *
 * Assuming two functors that work in both contexts, one of which always
 * returns true, the other of which always returns false, then:
 *
 * Static:
 *
 *   _and<_true, _true>::check(someparam)   // is true
 *   _and<_false, _true>::check(someparam)  // is false
 *   _or<_true, _true>::check(someparam)    // is true
 *   _or<_false, _true>::check(someparam)   // is true
 *
 * Dynamic:
 *
 *   _and<_true, _true> c1; c1(someparam);  // is true
 *   _and<_false, _true> c2; c2(someparam); // is false
 *   _or<_true, _true> c3; c3(someparam);   // is true
 *   _or<_false, _true> c4; c4(someparam);  // is true
 *
 * Conditions can be arbitrarily complex, e.g.
 *
 *   _and<_true, _or<_false, _true, _and<_true, _false>>>
 *
 * Conditions can take arbitrary parameters. The only requirement is that
 * all conditions used in a complex construct agree on the order and type
 * of them. Similarly, conditions can perform arbitrary checks, as long as
 * they always return a boolean value.
 *
 * The difference between static and dynamic contexts for a condition is
 * which member function gets called:
 *
 *    static bool mycondition::check(params...) in a static context
 *    bool mycondition::operator()(params...)   in a dynamic context
 *
 * A condition may implement both and may implement both differently. For
 * convenience, a helper construct "dynamic" provides an operator() that
 * maps to a static check function:
 *
 * struct _true
 *   : public meta::condition::dynamic<_true>
 * {
 *   static bool check()
 *   {
 *     return true;
 *   }
 * };
 **/
template <
  typename... Conditions
>
struct _and
  : public ::meta::types::compositionlist<Conditions...>
  , public detail::dynamic_chain_helper<detail::dynamic_and, Conditions...>

{
  inline _and(Conditions &&... initializers)
    : ::meta::types::compositionlist<Conditions...>(initializers...)
  {
  }


  template <typename... Args>
  static inline bool check(Args && ... args)
  {
    return detail::static_chain_helper<
      detail::dual_and,
      Conditions...
    >::check(std::forward<Args>(args)...);
  }


  template <typename... Args>
  inline bool operator()(Args && ... args)
  {
    return detail::dynamic_chain_helper<
      detail::dynamic_and,
      Conditions...
    >::operator()(std::forward<Args>(args)...);
  }
};



template <
  typename... Conditions
>
struct _or
  : public ::meta::types::compositionlist<Conditions...>
  , public detail::dynamic_chain_helper<detail::dynamic_or, Conditions...>
{
  inline _or(Conditions &&... initializers)
    : ::meta::types::compositionlist<Conditions...>(initializers...)
  {
  }


  template <typename... Args>
  static inline bool check(Args && ... args)
  {
    return detail::static_chain_helper<
      detail::dual_or,
      Conditions...
    >::check(std::forward<Args>(args)...);
  }


  template <typename... Args>
  inline bool operator()(Args && ... args)
  {
    return detail::dynamic_chain_helper<
      detail::dynamic_or,
      Conditions...
    >::operator()(std::forward<Args>(args)...);
  }
};

}} // namespace meta::condition

#endif // guard
