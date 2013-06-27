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

#include <meta/detail/dual_operators.tcc>
#include <meta/detail/chain_helper.tcc>


namespace meta {
namespace condition {

/**
 * This header defines two variadic templates, _and and _or for chaining
 * together functors that represent conditions via && and || operators.
 *
 * FIXME
 *
 *
 *
 **/
template <
  typename... Conditions
>
struct _and
{
  template <typename... Args>
  static inline bool check(Args... args)
  {
    return detail::chain_helper<
      detail::dual_and,
      Conditions...
    >::check(args...);
  }
};



template <
  typename... Conditions
>
struct _or
{
  template <typename... Args>
  static inline bool check(Args... args)
  {
    return detail::chain_helper<
      detail::dual_or,
      Conditions...
    >::check(args...);
  }
};

}} // namespace meta::condition

#endif // guard