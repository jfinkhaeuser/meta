/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.co.uk>
 *
 * Copyright (c) 2009-2012 Jens Finkhaeuser.
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
#ifndef META_COMPARISON_H
#define META_COMPARISON_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

namespace meta {

/**
 * This file defines constructs for comparing integers at compile-time. Usage
 * of all these constructs follows the same basic pattern, e.g.:
 *
 *    bool b = compare_construct<42, 666>::value;
 *
 * Depending on the comparison performed by the construct, the resulting value
 * evaluates either to true or to false.
 **/
template <int I1, int I2>
struct equal
{
  enum {
    value = (I1 == I2),
  };
};



template <int I1, int I2>
struct not_equal
{
  enum {
    value = (I1 != I2),
  };
};



template <int I1, int I2>
struct greater
{
  enum {
    value = (I1 > I2),
  };
};



template <int I1, int I2>
struct less
{
  enum {
    value = (I1 < I2),
  };
};



template <int I1, int I2>
struct greater_equal
{
  enum {
    value = (I1 >= I2),
  };
};



template <int I1, int I2>
struct less_equal
{
  enum {
    value = (I1 <= I2),
  };
};

} // namespace meta


#endif // guard
