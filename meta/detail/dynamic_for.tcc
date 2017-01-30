/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
 *
 * Copyright (c) 2009-2012 Jens Finkhaeuser.
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

#ifndef META_DETAIL_DYNAMIC_FOR_TCC
#define META_DETAIL_DYNAMIC_FOR_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/comparison.h>

namespace meta {


/*****************************************************************************
 * Helper structures
 **/

namespace detail {
namespace dynamic_for_helpers {

/**
 * Generic implementation of next is for cases where the TRUE_FALSE condition is
 * non-zero, i.e. true.
 *
 * In those cases, we first call the functor and pass the CURRENT value. Then we
 * instanciate a new next with CURRENT incremented, and pass control to that
 * instance's run function.
 **/
template <
  int TRUE_FALSE, // result of exit condition; next is specialized on this below
  int CURRENT,    // current value of the for loop counter
  int END,        // end of the for loop
  template <int> class incrementorT,
  template <typename intT, intT, intT> class comparatorT
>
struct next
{
  template <typename functorT>
  inline static void run(functorT & func)
  {
    func(CURRENT);
    next<
      comparatorT<int, incrementorT<CURRENT>::value, END>::value,
      incrementorT<CURRENT>::value,
      END,
      incrementorT,
      comparatorT
    >::run(func);
  }

  template <typename functorT>
  inline static void run(functorT const & func)
  {
    func(CURRENT);
    next<
      comparatorT<int, incrementorT<CURRENT>::value, END>::value,
      incrementorT<CURRENT>::value,
      END,
      incrementorT,
      comparatorT
    >::run(func);
  }
};


/**
 * Specialization for TRUE_FALSE == 0 - neither run function does anything, as
 * END is not included in the iterated range.
 **/
template <
  int CURRENT,
  int END,
  template <int> class incrementorT,
  template <typename intT, intT, intT> class comparatorT
>
struct next<0, CURRENT, END, incrementorT, comparatorT>
{
  template <typename functorT>
  inline static void run(functorT &)
  {
  }

  template <typename functorT>
  inline static void run(functorT const &)
  {
  }
};


}} // namespace detail::dynamic_for_helpers

/*****************************************************************************
 * Wrapper function implementations
 **/
template <
  int START,
  int END,
  typename functorT
>
inline void
dynamic_for(functorT const & func)
{
  detail::dynamic_for_helpers::next<
    ::meta::less<int, START, END>::value,
    START,
    END,
    ::meta::inc_once,
    ::meta::less
  >::template run<functorT>(func);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  typename functorT
>
inline void
dynamic_for(functorT const & func)
{
  detail::dynamic_for_helpers::next<
    ::meta::less<int, START, END>::value,
    START,
    END,
    incrementorT,
    ::meta::less
  >::template run<functorT>(func);
}


template <
  int START,
  int END,
  template <typename intT, intT, intT> class comparatorT,
  typename functorT
>
inline void
dynamic_for(functorT const & func)
{
  detail::dynamic_for_helpers::next<
    comparatorT<int, START, END>::value,
    START,
    END,
    ::meta::inc_once,
    comparatorT
  >::template run<functorT>(func);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <typename intT, intT, intT> class comparatorT,
  typename functorT
>
inline void
dynamic_for(functorT const & func)
{
  detail::dynamic_for_helpers::next<
    comparatorT<int, START, END>::value,
    START,
    END,
    incrementorT,
    comparatorT
  >::template run<functorT>(func);
}


template <
  int START,
  int END,
  typename functorT
>
inline void
dynamic_for(functorT & func)
{
  detail::dynamic_for_helpers::next<
    ::meta::less<int, START, END>::value,
    START,
    END,
    ::meta::inc_once,
    ::meta::less
  >::template run<functorT>(func);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  typename functorT
>
inline void
dynamic_for(functorT & func)
{
  detail::dynamic_for_helpers::next<
    ::meta::less<int, START, END>::value,
    START,
    END,
    incrementorT,
    ::meta::less
  >::template run<functorT>(func);
}


template <
  int START,
  int END,
  template <typename intT, intT, intT> class comparatorT,
  typename functorT
>
inline void
dynamic_for(functorT & func)
{
  detail::dynamic_for_helpers::next<
    comparatorT<int, START, END>::value,
    START,
    END,
    ::meta::inc_once,
    comparatorT
  >::template run<functorT>(func);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <typename intT, intT, intT> class comparatorT,
  typename functorT
>
inline void
dynamic_for(functorT & func)
{
  detail::dynamic_for_helpers::next<
    comparatorT<int, START, END>::value,
    START,
    END,
    incrementorT,
    comparatorT
  >::template run<functorT>(func);
}

} // namespace meta

#endif // guard
