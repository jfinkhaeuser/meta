/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
 *
 * Copyright (c) 2014-2015 Unwesen Ltd.
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

#ifndef META_DETAIL_CONTAINER_RESTRICTIONS_H
#define META_DETAIL_CONTAINER_RESTRICTIONS_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

namespace meta {
namespace restrictions {
namespace container {

/**
 * Enforces non-empty container types, where any valueT that has an empty()
 * function that returns true when the value is empty is considered a container
 * type.
 **/
struct non_empty
{
  template <
    typename valueT
  >
  static inline bool check(valueT const & value)
  {
    return !value.empty();
  }
};


/**
 * Enforces empty container types, where any valueT that has an empty() function
 * that returns true when the value is empty is considered a container type.
 **/
struct empty
{
  template <
    typename valueT
  >
  static inline bool check(valueT const & value)
  {
    return value.empty();
  }
};



}}} // namespace meta::restrictions::container

#endif // guard
