/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
 *
 * Copyright (c) 2009-2012 Jens Finkhaeuser.
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

#ifndef META_DETAIL_LIST_REVERT_H
#define META_DETAIL_LIST_REVERT_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

#include <meta/detail/typelist.h>
#include <meta/detail/inhlist.h>
#include <meta/detail/complist.h>
#include <meta/detail/list_append.h>

namespace meta {
namespace types {

/**
 * Reverts a list of types.
 **/
namespace detail {

template <
  template <typename...> class ListType,
  typename... Types
>
struct revert_helper;

template <
  template <typename...> class ListType
>
struct revert_helper<ListType>
{
  typedef ListType<> type;
};

template <
  template <typename...> class ListType,
  typename Head,
  typename... Tail
>
struct revert_helper<ListType, Head, Tail...>
{
  typedef typename append<
    Head,
    typename revert_helper<ListType, Tail...>::type
  >::type type;
};

} // namespace detail

template <
  typename Types
>
struct revert;

// Specialization for typelists
template <
  typename... Types
>
struct revert <
      typelist<Types...>
    >
{
  typedef typename detail::revert_helper<typelist, Types...>::type type;
};

// Specialization for inheritance lists
template <
  typename... Types
>
struct revert <
      inheritancelist<Types...>
    >
{
  typedef typename detail::revert_helper<inheritancelist, Types...>::type type;
};

// Specialization for composition lists
template <
  typename... Types
>
struct revert <
      compositionlist<Types...>
    >
{
  typedef typename detail::revert_helper<compositionlist, Types...>::type type;
};

}} // namespace meta::types

#endif // guard
