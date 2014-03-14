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

#ifndef META_DETAIL_DYNAMIC_TCC
#define META_DETAIL_DYNAMIC_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

namespace meta {
namespace condition {

/**
 * Very simple base class for making a static condition also work in a dynamic
 * context.
 **/
template <typename parentT>
struct dynamic
{
  template <typename... Args>
  inline bool operator()(Args && ... args) const
  {
    return parentT::check(args...);
  }
};


}} // namespace meta::condition

#endif // guard
