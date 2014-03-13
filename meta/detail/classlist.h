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
#ifndef META_DETAIL_CLASSLIST_H
#define META_DETAIL_CLASSLIST_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

namespace meta {
namespace types {

/**
 * This file contains meta-programming code for handling lists of types.
 **/

/**
 * The classlist struct is just used to represent a list of classes at compile
 * and run-time. Unlike typelist, it has a run-time value, namely the values
 * of all the types in it's template parameter list, since it derives from all
 * those types.
 **/
template <typename... Classes>
struct classlist;

template <typename Head, typename... Tail>
struct classlist<Head, Tail...>
  : public Head
  , public Tail...
{
  inline classlist()
  {
  }

  // FIXME
  // It's *really* strange, but it appears we have to put the head at the end
  // if we want classlist<int, float> to be initialized by { 42, 3.14 } rather
  // than { 3.14, 42 } (or rather classlist<int_type, float_type>).
  inline classlist(Head && head, Tail && ... tail)
    : Head(head)
    , Tail(tail)...
  {
  }

  virtual ~classlist() {}
};

// Specialization needed for empty classlist constructor
template <>
struct classlist<>
{
  inline classlist()
  {
  }

  virtual ~classlist() {}
};


}} // namespace meta::types

#endif // guard
