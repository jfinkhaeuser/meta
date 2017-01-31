/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
 *
 * Copyright (c) 2014 Unwesen Ltd.
 * Copyright (c) 2015-2017 Jens Finkhaeuser.
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

#include <ctime>

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
  std::clock_t current = std::clock();
  return static_cast<int64_t>(
      (static_cast<double>(current) / CLOCKS_PER_SEC)
      * 1000000L
  );
}

}} // namespace test::timing
