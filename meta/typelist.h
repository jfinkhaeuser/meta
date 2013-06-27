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
#ifndef META_TYPELIST_H
#define META_TYPELIST_H

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
 * Two types representing boolean true/false respectively.
 **/
struct true_type {};
struct false_type {};


/**
 * The typelist struct is just used to represent a list of types at compile
 * time; it has no run-time value.
 **/
template <typename... Types>
struct typelist {};


/**
 * The contains consturct returns true (contains<>::type is true_type) if the
 * type given in first template argument is part of the typelist given in the
 * second argument.
 *
 * Note that the second argument *must* be a typelist; if it's not, false_type
 * is returned.
 *
 * Example usage:
 *    contains<int, typelist<int, float, double>>::type // is true_type
 *    contains<float, typelist<int, char>>::type        // is false_type
 *
 * Bad usage:
 *    contains<int, int>::type // compiles, but is false_type
 **/

// Default to false.
template <typename, typename>
struct contains
{
  typedef false_type type;
};

// Matches if the first type in the typelist is the test type.
template <
  typename TestType,
  typename... Types
>
struct contains<
      TestType,
      typelist<TestType, Types...>
    >
{
  typedef true_type type;
};

// Matches if the first type in the typelist is not the test type, so we'll
// recursively try typelist elements until it's the first type and the above
// struct matches.
template <
  typename TestType,
  typename Head,
  typename... Tail
>
struct contains<
      TestType,
      typelist<Head, Tail...>
    >
{
  typedef typename contains<
    TestType,
    typelist<Tail...>
  >::type type;
};


/**
 * The append_unique construct's ::type is a typelist that is a unique set of the
 * first template argument passed to it and the types contained in the typelist
 * passed as the second template argument. That is, if the second argument
 * already contains the first argument, the second argument is returned
 * unaltered.
 *
 * Note also that the second argument must be a typelist.
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



/**
 * The unique_typelist construct puts it all together. It produces a typelist
 * of unique types.
 *
 * Note that this construct uses append_unique internally, the unique typelist
 * will contain elements in the order of first appearance.
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
  typedef typename append_unique<
    Head,
    typename unique_typelist<Tail...>::type
  >::type type;
};


}} // namespace meta::types

#endif // guard
