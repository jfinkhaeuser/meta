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
#ifndef META_DETAIL_PREPEND_UNIQUE_H
#define META_DETAIL_PREPEND_UNIQUE_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

#include <meta/detail/typelist.h>
#include <meta/detail/classlist.h>
#include <meta/detail/list_contains.h>

namespace meta {
namespace types {

/**
 * This file contains meta-programming code for handling lists of types.
 **/
template <
  typename NewType,
  typename Types,
  typename Duplicate = typename contains<NewType, Types>::type
>
struct prepend_unique;

// Matches if Duplicate is true_type
template <
  typename NewType,
  typename... Types
>
struct prepend_unique <
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
struct prepend_unique <
      NewType,
      typelist<Types...>,
      false_type
    >
{
  typedef typelist<NewType, Types...> type;
};




// Same for classes; true_type
template <
  typename NewType,
  typename... Types
>
struct prepend_unique <
      NewType,
      classlist<Types...>,
      true_type
    >
{
  typedef classlist<Types...> type;
};

// Same for classes; false_type
template <
  typename NewType,
  typename... Types
>
struct prepend_unique <
      NewType,
      classlist<Types...>,
      false_type
    >
{
  typedef classlist<NewType, Types...> type;
};


}} // namespace meta::types

#endif // guard
