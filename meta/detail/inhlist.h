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

#ifndef META_DETAIL_INHERITANCELIST_H
#define META_DETAIL_INHERITANCELIST_H

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
 * The inheritancelist struct is just used encapsulate a list of classes at
 * compile and run-time, by inheriting from every class passed to the list.
 *
 * Due to compiler/language restrictions one cannot inherit from the same
 * base class twice, which means inheritancelist only accepts each type
 * once. In situations where you're unsure what types you're getting, you
 * might want to use the make_unique construct.
 *
 * Unlike typelist, inheritance list has run-time values, namely the values
 * of all the types in it's template parameter list.
 **/
template <typename... Classes>
struct inheritancelist;

template <typename Head, typename... Tail>
struct inheritancelist<Head, Tail...>
  : public Head
  , public Tail...
{
  inline inheritancelist()
  {
  }

  // XXX There were some problems with early GCC support that seemed to have
  //     reversed the initalizer list if it was passed in curly braces. It
  //     can't be reproduced any longer (GCC 4.6.3).
  inline inheritancelist(Head && head, Tail && ... tail)
    : Head(head)
    , Tail(tail)...
  {
  }

  virtual ~inheritancelist() {}
};

// Specialization needed for empty inheritancelist constructor
template <>
struct inheritancelist<>
{
  inline inheritancelist()
  {
  }

  virtual ~inheritancelist() {}
};


}} // namespace meta::types

#endif // guard
