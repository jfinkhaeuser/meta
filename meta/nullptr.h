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

#ifndef META_NULLPTR_H
#define META_NULLPTR_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif


/*****************************************************************************
 * Incomplete c++11 implementations on older compilers force us to fake
 * nullptr
 **/
#if defined(__GNUC__) && __GNUC_MINOR__ < 6
#  define META_NEED_NULLPTR_COMPATIBILITY
#endif


/*****************************************************************************
 * nullptr compatibility
 **/
#if defined(META_NEED_NULLPTR_COMPATIBILITY)
const                         // this is a const object...
class {
public:
  template<class T>           // convertible to any type
  operator T*() const         // of null non-member
  { return 0; }               // pointer...
  template<class C, class T>  // or any type of null
  operator T C::*() const     // member pointer...
  { return 0; }
private:
  void operator&() const;     // whose address can't be taken
} nullptr = {};
#endif // META_NEED_NULLPTR_COMPATIBILITY

#endif // guard
