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

#ifndef META_STACKONLY_H
#define META_STACKONLY_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

namespace meta {
namespace detail {

#if META_CXX_MODE == META_CXX_MODE_CXX0X
#  define META_DELETE = delete
#  define META_NOEXCEPT noexcept
#else
#  define META_DELETE
#  define META_NOEXCEPT throw()
#endif

#if defined(META_WIN32)
#  define META_THROW_BAD_ALLOC
#else
#  if META_CXX_MODE == META_CXX_MODE_CXX0X
#    define META_THROW_BAD_ALLOC throw()
#  else
#    define META_THROW_BAD_ALLOC throw (std::bad_alloc)
#  endif
#endif


/**
 * Private new operator ensures classes derived from stackonly cannot be
 * created on the heap.
 **/
class stackonly
{
protected:
  inline stackonly()
  {
  }

  inline ~stackonly()
  {
  }


private:
  // The whole point is for the following to be private.
  void * operator new (std::size_t size) META_THROW_BAD_ALLOC META_DELETE;
  void * operator new (std::size_t size, std::nothrow_t const & nothrow_value) META_NOEXCEPT META_DELETE;
};

#undef META_DELETE

} // namespace meta::detail

// Protect from unintended Koenig lookup
typedef detail::stackonly stackonly;

} // namespace meta

#endif // guard
