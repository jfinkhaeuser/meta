/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.de>
 *
 * Copyright (c) 2009-2012 Jens Finkhaeuser.
 * Copyright (c) 2013-2015 Unwesen Ltd.
 * Copyright (c) 2016 Jens Finkhaeuser
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

#ifndef META_DETAIL_APPEND_H
#define META_DETAIL_APPEND_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

#include <meta/detail/typelist.h>
#include <meta/detail/inhlist.h>
#include <meta/detail/complist.h>

namespace meta {
namespace types {

/**
 * This file contains meta-programming code for handling lists of types.
 **/

/**
 * The append construct's ::type is a typelist that contains the first template
 * argument passed to it and the types contained in the typelist passed as the
 * second template argument. Ordering is preserved.
 *
 * Example usage:
 *    append<int, typelist<float>>::type      // is typelist<float, int>
 *    append<int, typelist<int, float>>::type // is typelist<int, float, int>
 **/

template <
  typename NewType,
  typename Types
>
struct append;

template <
  typename NewType,
  typename... Types
>
struct append <
      NewType,
      typelist<Types...>
    >
{
  typedef typelist<Types..., NewType> type;
};



// Same for inheritance lists
template <
  typename NewType,
  typename... Types
>
struct append <
      NewType,
      inheritancelist<Types...>
    >
{
  typedef inheritancelist<Types..., NewType> type;
};



// Same for composition lists
template <
  typename NewType,
  typename... Types
>
struct append <
      NewType,
      compositionlist<Types...>
    >
{
  typedef compositionlist<Types..., NewType> type;
};


}} // namespace meta::types

#endif // guard
