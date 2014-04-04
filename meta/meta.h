/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.co.uk>
 *
 * Copyright (c) 2014 Unwesen Ltd.
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


#endif // guard
