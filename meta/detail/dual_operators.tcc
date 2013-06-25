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

#ifndef META_DETAIL_DUAL_OPERATORS_TCC
#define META_DETAIL_DUAL_OPERATORS_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

namespace meta {
namespace condition {
namespace detail {


template <
  typename Left,
  typename Right
>
struct dual_and
{
  template <typename ...Args>
  static inline bool check(Args... args)
  {
    return Left::check(args...) && Right::check(args...);
  }
};



template <
  typename Left,
  typename Right
>
struct dual_or
{
  template <typename ...Args>
  static inline bool check(Args... args)
  {
    return Left::check(args...) || Right::check(args...);
  }
};


}}} // namespace meta::condition::detail

#endif // guard
