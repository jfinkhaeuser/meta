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
#ifndef META_DETAIL_APPEND_UNIQUE_H
#define META_DETAIL_APPEND_UNIQUE_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

#include <meta/detail/typelist.h>
#include <meta/detail/inhlist.h>
#include <meta/detail/complist.h>
#include <meta/detail/list_contains.h>

namespace meta {
namespace types {

/**
 * This file contains meta-programming code for handling lists of types.
 **/

/**
 * The append_unique construct's ::type is a typelist that is a unique set of the
 * first template argument passed to it and the types contained in the typelist
 * passed as the second template argument. That is, if the second argument
 * already contains the first argument, the second argument is returned
 * unaltered.
 *
 * Note also that the second argument must be a typelist.
 *
 * Variants exist for inheritance and composition lists.
 *
 * Example usage:
 *    append_unique<int, typelist<float>>::type      // is typelist<float, int>
 *    append_unique<int, typelist<int, float>>::type // is typelist<int, float>
 *
 * Bad usage:
 *    append_unique<int, int>::type // does not compile
 **/

// Generic templat edefinition. The third parameter determines which of the
// two specializations below will be matched.
template <
  typename NewType,
  typename Types,
  typename Duplicate = typename contains<NewType, Types>::type
>
struct append_unique;

// Matches if Duplicate is true_type
template <
  typename NewType,
  typename... Types
>
struct append_unique <
      NewType,
      typelist<Types...>,
      true_type
    >
{
  typedef typelist<Types...> type;
};

// Matches if Duplicate is false_type
template <
  typename NewType,
  typename... Types
>
struct append_unique <
      NewType,
      typelist<Types...>,
      false_type
    >
{
  typedef typelist<Types..., NewType> type;
};




// Same for inheritnace lists; true_type
template <
  typename NewType,
  typename... Types
>
struct append_unique <
      NewType,
      inheritancelist<Types...>,
      true_type
    >
{
  typedef inheritancelist<Types...> type;
};

// Same for inheritance; false_type
template <
  typename NewType,
  typename... Types
>
struct append_unique <
      NewType,
      inheritancelist<Types...>,
      false_type
    >
{
  typedef inheritancelist<Types..., NewType> type;
};




// Same for inheritnace lists; true_type
template <
  typename NewType,
  typename... Types
>
struct append_unique <
      NewType,
      compositionlist<Types...>,
      true_type
    >
{
  typedef compositionlist<Types...> type;
};

// Same for composition; false_type
template <
  typename NewType,
  typename... Types
>
struct append_unique <
      NewType,
      compositionlist<Types...>,
      false_type
    >
{
  typedef compositionlist<Types..., NewType> type;
};

}} // namespace meta::types

#endif // guard
