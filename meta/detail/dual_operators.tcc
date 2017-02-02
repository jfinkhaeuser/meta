/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
 *
 * Copyright (c) 2013-2015 Unwesen Ltd.
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

#ifndef META_DETAIL_DUAL_OPERATORS_TCC
#define META_DETAIL_DUAL_OPERATORS_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <utility>

namespace meta {
namespace condition {
namespace detail {

/**
 * Dual operators for static chains
 **/
template <
  typename Left,
  typename Right
>
struct dual_and
{
  template <typename... Args>
  static inline bool check(Args && ... args)
  {
    return Left::check(std::forward<Args>(args)...)
      && Right::check(std::forward<Args>(args)...);
  }
};




template <
  typename Left,
  typename Right
>
struct dual_or
{
  template <typename... Args>
  static inline bool check(Args && ... args)
  {
    return Left::check(std::forward<Args>(args)...)
      || Right::check(std::forward<Args>(args)...);
  }
};




/**
 * Recursive operators for dynamic chains
 **/
template <
  typename Left,
  typename Right,
  typename... Tail
>
struct dynamic_and
  : public Right
{
  template <typename Owner, typename... Args>
  inline bool operator()(Owner * owner, Args && ... args)
  {
    // XXX We know that we can cast to compositionlist because of details in
    //     condition.h - make sure that stays in sync!
    return reinterpret_cast<
      meta::types::compositionlist<Left, Tail...>
    *>(owner)->item.operator()(std::forward<Args>(args)...)
      && Right::operator()(owner, std::forward<Args>(args)...);
    return true;
  }
};


template <typename Left, typename Right>
struct dynamic_and<Left, Right>
{
  template <typename Owner, typename... Args>
  inline bool operator()(Owner * owner, Args && ... args)
  {
    // Right-hand side is unused.
    // XXX We know that we can cast to compositionlist because of details in
    //     condition.h - make sure that stays in sync!
    return reinterpret_cast<
      meta::types::compositionlist<Left> *
    >(owner)->item.operator()(std::forward<Args>(args)...);
  }

};





template <
  typename Left,
  typename Right,
  typename... Tail
>
struct dynamic_or
  : public Right
{  
  template <typename Owner, typename... Args>
  inline bool operator()(Owner * owner, Args && ... args)
  {
    // XXX We know that we can cast to compositionlist because of details in
    //     condition.h - make sure that stays in sync!
    return reinterpret_cast<
      meta::types::compositionlist<Left, Tail...>
    *>(owner)->item.operator()(std::forward<Args>(args)...)
        || Right::operator()(owner, std::forward<Args>(args)...);
    return true;
  }
};


template <typename Left, typename Right>
struct dynamic_or<Left, Right>
{
  template <typename Owner, typename... Args>
  inline bool operator()(Owner * owner, Args && ... args)
  {
    // Right-hand side is unused.
    // XXX We know that we can cast to compositionlist because of details in
    //     condition.h - make sure that stays in sync!
    return reinterpret_cast<
      meta::types::compositionlist<Left> *
    >(owner)->item.operator()(std::forward<Args>(args)...);
  }

};



}}} // namespace meta::condition::detail

#endif // guard
