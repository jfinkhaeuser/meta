/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
 *
 * Copyright (c) 2013-2015 Unwesen Ltd.
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

#ifndef META_NULLPTR_H
#define META_NULLPTR_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

/*****************************************************************************
 * nullptr compatibility
 **/
#if !defined(META_HAVE_STD_NULLPTR_T

/**
 * nullptr emulation class similar to Effective C++ by Scott Meyers,
 * Second Edition.
 */
namespace std {

class nullptr_t
{
public:
  /**
   * Can convert to any null non-member pointer.
   **/
  template <typename T>
  inline operator T*() const
  {
    return 0;
  }

  /**
   * Can convert to null member pointers.
   **/
  template <typename C, typename T>
  inline operator T C::*() const
  {
    return 0;
  }

private:
  /**
   * Can't take the address of an object.
   **/
  void operator&() const;
};

} // namespace std


#endif // META_HAVE_STD_NULLPTR_T


#if !defined(META_HAVE_NULLPTR)

std::nullptr_t const nullptr = {};

#endif // META_HAVE_NULLPTR

#endif // guard
