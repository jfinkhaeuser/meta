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
#ifndef META_DETAIL_LIST_VISIT_H
#define META_DETAIL_LIST_VISIT_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

#include <utility>

#include <meta/detail/typelist.h>
#include <meta/detail/inhlist.h>
#include <meta/detail/complist.h>

namespace meta {
namespace types {

namespace detail {

template <
  typename ListType,
  typename... Types
>
struct apply_func;

// Specialization for composition lists
template <
  typename... Types
>
struct apply_func<compositionlist<Types...>>
{
  template <
    template <typename> class FuncType,
    typename... Args
  >
  inline static void invoke(compositionlist<Types...> const & list,
      FuncType func, Args && ... args)
  {
    std::cout << "list: " << typeid(list).name() << std::endl;
    std::cout << typeid(list.item).name() << std::endl;
    func(list.item, std::forward<Args>(args)...);
  }
};


// Generic helper
template <
  template <typename...> class ListType,
  typename... Types
>
struct visitor_helper;


// Recursion end
template <
  template <typename...> class ListType
>
struct visitor_helper<ListType>
{
  template <
    template <typename> class FuncType,
    typename... Args
  >
  inline static void invoke(ListType<> const &, FuncType, Args && ...)
  {
    // End of run-time recursion
  }
};


// Recursion
template <
  template <typename...> class ListType,
  typename Head,
  typename... Tail
>
struct visitor_helper<ListType, Head, Tail...>
{
  template <
    template <typename> class FuncType,
    typename... Args
  >
  inline static void invoke(ListType<Head, Tail...> const & list,
      FuncType func, Args && ... args)
  {
    apply_func<ListType<Head, Tail...>>::invoke(list, func, std::forward<Args>(args)...);
    visitor_helper<ListType, Tail...>::invoke(list, func, std::forward<Args>(args)...);
  }
};


} // namespace detail


}} // namespace meta::types

#endif // guard
