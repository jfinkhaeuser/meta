/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.co.uk>
 *
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
#ifndef META_NONCOPYABLE_H
#define META_NONCOPYABLE_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

namespace meta {
namespace detail {

/**
 * Private copy ctor and assignment ensure classes derived from noncopyable
 * cannot be copied.
 **/
class noncopyable
{
protected:
  inline noncopyable()
  {
  }

  inline ~noncopyable()
  {
  }


private:
  // The whole point is for the following to be private.
  noncopyable(noncopyable const &) = delete;
  noncopyable & operator=(noncopyable const &) = delete;
};

} // namespace meta::detail

// Protect from unintended Koenig lookup
typedef detail::noncopyable noncopyable;

} // namespace meta

#endif // guard
