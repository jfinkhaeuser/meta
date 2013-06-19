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
#ifndef META_DETAIL_STATIC_FOR_TCC
#define META_DETAIL_STATIC_FOR_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

namespace meta {


/*****************************************************************************
 * Helper structures
 **/

namespace detail {
namespace static_for_helpers {

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
  template <int, int> class comparatorT,
  template <int> class functorT
>
struct next
{
  inline static void run()
  {
    functorT<CURRENT> func;
    func();
    next<
      comparatorT<incrementorT<CURRENT>::value, END>::value,
      incrementorT<CURRENT>::value,
      END,
      incrementorT,
      comparatorT,
      functorT
    >::run();
  }

  template <typename functor_paramT>
  inline static void run(functor_paramT & param)
  {
    functorT<CURRENT> func;
    func(param);
    next<
      comparatorT<incrementorT<CURRENT>::value, END>::value,
      incrementorT<CURRENT>::value,
      END,
      incrementorT,
      comparatorT,
      functorT
    >::run(param);
  }

  template <typename functor_paramT>
  inline static void run(functor_paramT const & param)
  {
    functorT<CURRENT> func;
    func(param);
    next<
      comparatorT<incrementorT<CURRENT>::value, END>::value,
      incrementorT<CURRENT>::value,
      END,
      incrementorT,
      comparatorT,
      functorT
    >::run(param);
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
  template <int, int> class comparatorT,
  template <int> class functorT
>
struct next<0, CURRENT, END, incrementorT, comparatorT, functorT>
{
  inline static void run()
  {
  }

  template <typename functor_paramT>
  inline static void run(functor_paramT &)
  {
  }

  template <typename functor_paramT>
  inline static void run(functor_paramT const &)
  {
  }
};





}} // namespace detail::static_for_helpers

/*****************************************************************************
 * Wrapper function implementations
 **/

/**
 * No parameter
 **/
template <
  int START,
  int END,
  template <int> class functorT
>
inline void
static_for()
{
  detail::static_for_helpers::next<
    ::meta::less<START, END>::value,
    START,
    END,
    ::meta::inc_once,
    ::meta::less,
    functorT
  >::template run();
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int> class functorT
>
inline void
static_for()
{
  detail::static_for_helpers::next<
    ::meta::less<START, END>::value,
    START,
    END,
    incrementorT,
    ::meta::less,
    functorT
  >::template run();
}


template <
  int START,
  int END,
  template <int, int> class comparatorT,
  template <int> class functorT
>
inline void
static_for()
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    ::meta::inc_once,
    comparatorT,
    functorT
  >::template run();
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int, int> class comparatorT,
  template <int> class functorT
>
inline void
static_for()
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    incrementorT,
    comparatorT,
    functorT
  >::template run();
}



/**
 * Parameter by reference
 **/
template <
  int START,
  int END,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param)
{
  detail::static_for_helpers::next<
    ::meta::less<START, END>::value,
    START,
    END,
    ::meta::inc_once,
    ::meta::less,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param)
{
  detail::static_for_helpers::next<
    ::meta::less<START, END>::value,
    START,
    END,
    incrementorT,
    ::meta::less,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int, int> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param)
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    ::meta::inc_once,
    comparatorT,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int, int> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param)
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    incrementorT,
    comparatorT,
    functorT
  >::template run(param);
}


/**
 * Parameter by const reference
 **/
template <
  int START,
  int END,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param)
{
  detail::static_for_helpers::next<
    ::meta::less<START, END>::value,
    START,
    END,
    ::meta::inc_once,
    ::meta::less,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param)
{
  detail::static_for_helpers::next<
    ::meta::less<START, END>::value,
    START,
    END,
    incrementorT,
    ::meta::less,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int, int> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param)
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    ::meta::inc_once,
    comparatorT,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int, int> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param)
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    incrementorT,
    comparatorT,
    functorT
  >::template run(param);
}



} // namespace meta

#endif // guard
