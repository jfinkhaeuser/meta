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
#ifndef META_DETAIL_UNIQUE_LIST_H
#define META_DETAIL_UNIQUE_LIST_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

#include <meta/detail/typelist.h>
#include <meta/detail/classlist.h>
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
 * There also exists a unique_classlist construct for classlists.
 *
 * Example usage:
 *    unique_typelist<int, int>::type         // is typelist<int>
 *    unique_typelist<int, float>::type       // is typelist<int, float>
 *    unique_typelist<float, int>::type       // is typelist<float, int>
 *    unique_typelist<int, float, int>::type  // is typelist<int, float>
 **/

// The generic prototype
template <typename... Types>
struct unique_typelist;

// End of recursion, and match for an empty list of types.
template <>
struct unique_typelist<>
{
  typedef typelist<> type;
};

// Split types into head and tail, and recurse.
template <
  typename Head,
  typename... Tail
>
struct unique_typelist<Head, Tail...>
{
  typedef typename prepend_unique<
    Head,
    typename revert<
      typename unique_typelist<Tail...>::type
    >::type
  >::type type;
};


// Same for classes
template <typename... Types>
struct unique_classlist;

// End of recursion, and match for an empty list of types.
template <>
struct unique_classlist<>
{
  typedef classlist<> type;
};

// Split types into head and tail, and recurse.
template <
  typename Head,
  typename... Tail
>
struct unique_classlist<Head, Tail...>
{
  typedef typename prepend_unique<
    Head,
    typename revert<
      typename unique_classlist<Tail...>::type
    >::type
  >::type type;
};


}} // namespace meta::types

#endif // guard
