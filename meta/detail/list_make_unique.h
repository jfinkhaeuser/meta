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
#ifndef META_DETAIL_LIST_MAKE_UNIQUE_H
#define META_DETAIL_LIST_MAKE_UNIQUE_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

#include <meta/detail/typelist.h>
#include <meta/detail/inhlist.h>
#include <meta/detail/complist.h>
#include <meta/detail/list_prepend_unique.h>
#include <meta/detail/list_revert.h>


namespace meta {
namespace types {

/**
 * This file contains meta-programming code for handling lists of types.
 **/

/**
 * The unique_typelist construct puts it all together. It produces a typelist
 * of unique types.
 *
 * Note that this construct uses prepend_unique internally, the unique typelist
 * will contain elements in the order of first appearance.
 *
 * The construct works for typelists, inheritance and composition lists.
 *
 * Example usage:
 *    make_unique<typelist, int, int>::type           // is typelist<int>
 *    make_unique<inheritancelist, int, float>::type  // is inheritancelist<int, float>
 *    make_unique<compositionlist, float, int>::type  // is compositionlist<float, int>
 *    make_unique<typelist, int, float, int>::type    // is typelist<int, float>
 **/

// The generic prototype
template <
  template <typename...> class ListType,
  typename... Types
>
struct make_unique;

// End of recursion, and match for an empty list of types.
template <
  template <typename...> class ListType
>
struct make_unique<ListType>
{
  typedef ListType<> type;
};

// Split types into head and tail, and recurse.
template <
  template <typename...> class ListType,
  typename Head,
  typename... Tail
>
struct make_unique<ListType, Head, Tail...>
{
  typedef typename prepend_unique<
    Head,
    typename revert<
      typename make_unique<ListType, Tail...>::type
    >::type
  >::type type;
};


}} // namespace meta::types

#endif // guard
