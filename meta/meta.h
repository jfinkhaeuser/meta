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

#ifndef META_META_H
#define META_META_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

/**
 * Define recognized modes
 **/
#undef  META_CXX_MODE_CXX98
#define META_CXX_MODE_CXX98   0x1998

#undef  META_CXX_MODE_CXX11
#define META_CXX_MODE_CXX11   0x2011

#undef  META_CXX_MODE_CXX0X
#define META_CXX_MODE_CXX0X   META_CXX_MODE_CXX11


/**
 * Define default mode - you can override, if you want
 **/
#ifndef META_CXX_MODE
#define META_CXX_MODE         META_CXX_MODE_CXX11
#endif

/**
 * Which platform are we on?
 **/
#if !defined(META_PLATFORM_DEFINED)
  #if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define META_WIN32
  #else
    #define META_POSIX
  #endif
  #define META_PLATFORM_DEFINED
#endif

/**
 * Decide what to include globally
 **/
#if defined(META_WIN32)
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #define __UNDEF_LEAN_AND_MEAN
  #endif
  #include <windows.h>
  #ifdef __UNDEF_LEAN_AND_MEAN
    #undef WIN32_LEAN_AND_MEAN
    #undef __UNDEF_LEAN_AND_MEAN
  #endif
#endif

/**
 * Enum classes may be supported in C++11 mode, and are required in some cases
 * on Windows. We'd like to abort compilation on Windows if not in C++11 mode,
 * and otherwise just do the right thing.
 **/
#if defined(META_WIN32)
#  if META_CXX_MODE < META_CXX_MODE_CXX11
#    define META_ENUM_CLASS(name, type) this_will_not_compile_on_windows_without_cxx11_support
#    define META_ENUM_CLASS_NS(name) this_will_not_compile_on_windows_without_cxx11_support
#  else
#    define META_ENUM_CLASS(name, type) class name : type
#    define META_ENUM_CLASS_NS(name) :: name
#  endif
#else
#  define META_ENUM_CLASS(name, type)
#  define META_ENUM_CLASS_NS(name)
#endif


#endif // guard
