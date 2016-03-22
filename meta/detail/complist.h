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

#ifndef META_DETAIL_COMPOSITIONLIST_H
#define META_DETAIL_COMPOSITIONLIST_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

namespace meta {
namespace types {

/**
 * This file contains meta-programming code for handling lists of types.
 **/

/**
 * The compositionlist construct composes all types into members, and therefore
 * has value much like inheritancelist. But because it uses composition instead
 * of inheritance, it can be used with POD types.
 **/
template <typename... Classes>
struct compositionlist;

// Tail of the recursion
template <>
struct compositionlist<>
{
  virtual ~compositionlist() {}
};

// Recursion
template <typename Head, typename... Tail>
struct compositionlist<Head, Tail...>
  : public compositionlist<Tail...>
{
  Head item;

  inline compositionlist(Head const & _item, Tail const & ... tail)
    : compositionlist<Tail...>(tail...)
    , item(_item)
  {
  }


  inline compositionlist(Head && _item, Tail && ... tail)
    : compositionlist<Tail...>(tail...)
    , item(_item)
  {
  }


  inline compositionlist()
    : compositionlist<Tail...>()
    , item()
  {
  }


  virtual ~compositionlist() {}
};


}} // namespace meta::types

#endif // guard
