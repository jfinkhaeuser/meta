/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
 *
 * Copyright (c) 2015 Jens Finkhaeuser.
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
#ifndef META_INTTYPES_H
#define META_INTTYPES_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

#if defined(META_HAVE_STD_HEADERS)
#  include <cstdint>
#  include <cinttypes>
#elif defined(META_HAVE_TR1_HEADERS)
#  include <tr1/cstdint>
#  include <tr1/cinttypes>
#else
#  include <stdint.h>
#  include <inttypes.h>
#endif

#endif // guard
