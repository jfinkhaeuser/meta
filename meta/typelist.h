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

#ifndef META_TYPELIST_H
#define META_TYPELIST_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

#if META_CXX_MODE != META_CXX_MODE_CXX0X
#error Can't compile meta/typelist.h because there's no C++11 support.
#endif

#include <meta/detail/typelist.h>
#include <meta/detail/inhlist.h>
#include <meta/detail/complist.h>
#include <meta/detail/list_contains.h>
#include <meta/detail/list_revert.h>
#include <meta/detail/list_append.h>
#include <meta/detail/list_append_unique.h>
#include <meta/detail/list_prepend.h>
#include <meta/detail/list_prepend_unique.h>
#include <meta/detail/list_make_unique.h>

#endif // guard
