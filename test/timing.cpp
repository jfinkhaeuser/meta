/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.de>
 *
 * Copyright (c) 2014 Unwesen Ltd.
 * Copyright (c) 2015-2016 Jens Finkhaeuser
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

#include <meta/meta-config.h>

#include <test/timing.h>

#include <iostream>

#include <sys/time.h>
#include <sys/resource.h>

#include <errno.h>
#include <string.h>


namespace test {
namespace timing {

timer::timer()
  : m_last(get_current())
{
}



timer::~timer()
{
}



int64_t
timer::duration()
{
  int64_t cur = get_current();
  int64_t result = cur - m_last;
  m_last = cur;
  return result;
}


int64_t
timer::get_current() const
{
  struct ::rusage usage;
  int err = ::getrusage(RUSAGE_SELF, &usage);
  if (err < 0) {
    std::cerr << "Failed to get rusage: " << ::strerror(errno) << std::endl;
    return -1;
  }

  int64_t user = (static_cast<int64_t>(usage.ru_utime.tv_sec) * 1000000L)
    + static_cast<int64_t>(usage.ru_utime.tv_usec);

  return user;
}

}} // namespace test::timing
