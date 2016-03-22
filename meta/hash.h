/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.de>
 *
 * Copyright (c) 2014-2015 Unwesen Ltd.
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

#ifndef META_HASH_H
#define META_HASH_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

#include <functional>
#include <cstddef>

namespace meta {
namespace hash {

/**
 * Hash multiple values
 **/
template <typename T>
inline std::size_t multi_hash(T const & t)
{
  return std::hash<T>()(t);
}

template <typename T0, typename... Ts>
inline std::size_t multi_hash(T0 const & t0, Ts && ... ts)
{
  std::size_t seed = multi_hash(t0);
  if (0 == sizeof...(ts)) {
    return seed;
  }

  size_t remainder = multi_hash(std::forward<Ts>(ts)...);

  // Based on boost::hash_combine, which is based on
  // http://www.cs.rmit.edu.au/~jz/fulltext/jasist-tch.pdf
  seed ^= remainder + 0x9e3779b9
    + (seed << 6) + (seed >> 2);

  return seed;
}

}} // namespace meta::hash

#endif // guard
