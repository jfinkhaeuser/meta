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

#ifndef META_RANGE_H
#define META_RANGE_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

#include <utility>

namespace meta {

/**
 * This class provides a *simple* way for using a pair of iterators (forming
 * a range) instead of a container in C++0x's new for loops. For a more complete
 * imlementation of ranges, see boost::range or
 * https://code.google.com/p/cxx1y-range/
 *
 * Use make_range for ease of use; make_range can also convert from a std::pair,
 * such as returned by multimap::equal_range(), etc.
 **/
template <typename iterT>
struct range
{
  constexpr range(iterT _begin, iterT _end)
    : m_begin(_begin)
    , m_end(_end)
  {
  }

  constexpr iterT begin() const
  {
    return m_begin;
  }

  constexpr iterT end() const
  {
    return m_end;
  }

  iterT m_begin;
  iterT m_end;
};


template <typename iterT>
range<iterT>
make_range(iterT begin, iterT end)
{
  return range<iterT>(begin, end);
}

template <typename iterT>
range<iterT>
make_range(std::pair<iterT, iterT> pair)
{
  return range<iterT>(pair.first, pair.second);
}

} // namespace meta

#endif // guard
