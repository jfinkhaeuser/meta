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
#ifndef META_FOR_H
#define META_FOR_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

namespace meta {

/**
 * Some possible constructs for incrementing the counter in the compile-time for
 * loops below.
 **/
template <int CURRENT, int DIFF>
struct increment
{
  enum {
    value = CURRENT + DIFF,
  };
};


template <int CURRENT, int DIFF>
struct decrement
{
  enum {
    value = CURRENT - DIFF,
  };
};


template <int CURRENT>
struct inc_once : public increment<CURRENT, 1>
{
};


template <int CURRENT>
struct dec_once : public decrement<CURRENT, 1>
{
};


template <int CURRENT, int FACTOR>
struct multiply
{
  enum {
    value = CURRENT * FACTOR,
  };
};


template <int CURRENT>
struct multi_double : public multiply<CURRENT, 2>
{
};


template <int CURRENT, int DIVISOR>
struct divide
{
  enum {
    value = CURRENT / DIVISOR,
  };
};





/**
 * Of all metaprogramming techniques, a compile-time for loop is probably the
 * best explored. This file offers two implementations with slightly different
 * use-cases. They're probably not much better or worse than other
 * implementations out there, but at least they don't pull in additional
 * dependencies.
 *
 * The first implementation, dynamic_for, iterates at compile-time over a range
 * of integer values defined by [START, END). For each of the values, a functor
 * is called, to which the current iteration value is passed as a run-time
 * parameter:
 *
 *    void functor(int i);
 *    dynamic_for<0, 10>(&functor);
 *
 *    struct functor2
 *    {
 *      void operator()(int i);
 *    };
 *    functor2 f;
 *    dynamic_for<0, 10>(f);
 *
 * In the above examples, the functor is called 10 times, with values ranging
 * from 0 to 9. If you wish to pass more values than the current iteration
 * value to the functor, you should probably take a look at boost::bind.
 *
 * The dynamic_for construct allows for an optional template parameter to change
 * how the loop increments the current iteration value. By default, the inc_once
 * structure above is used.
 *
 * Finally, dynamic_for allows for an optional comparator template parameter, to
 * change how the current loop value is compared to the end loop value. By
 * default, a less operation is performed, to follow the runtime for loop
 * semantics. Alternatively, you can also choose to supply any of the
 * comparators from meta/comparison.h, or your custom constructs.
 **/
template <
  int START,
  int END,
  typename functorT
>
inline void
dynamic_for(functorT const & func);


template <
  int START,
  int END,
  template <int> class incrementorT,
  typename functorT
>
inline void
dynamic_for(functorT const & func);


template <
  int START,
  int END,
  template <typename intT, intT, intT> class comparatorT,
  typename functorT
>
inline void
dynamic_for(functorT const & func);


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <typename intT, intT, intT> class comparatorT,
  typename functorT
>
inline void
dynamic_for(functorT const & func);


template <
  int START,
  int END,
  typename functorT
>
inline void
dynamic_for(functorT & func);


template <
  int START,
  int END,
  template <int> class incrementorT,
  typename functorT
>
inline void
dynamic_for(functorT & func);


template <
  int START,
  int END,
  template <typename intT, intT, intT> class comparatorT,
  typename functorT
>
inline void
dynamic_for(functorT & func);


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <typename intT, intT, intT> class comparatorT,
  typename functorT
>
inline void
dynamic_for(functorT & func);




/**
 * The second implementation, static_for, performs the same iteration. Instead
 * of calling the same functor over and over, though, it instanciates a class
 * template you specify with the current iteration value as it's template
 * parameter:
 *
 *    template <int I>
 *    struct functor
 *    {
 *      void operator()();
 *    };
 *
 *    static_for<0, 10, functor>();
 *
 * In the above example, a functor<0>, functor<1>, ..., functor<9> are
 * instanciated and their respective operator()s called. Note that static_for
 * only works with struct-based functors, not first-class functions.
 *
 * If you wish to pass additional parameters to the functor's operator()s, there
 * is support for an additional parameter. If you wish to pass more than one
 * additional parameters, consider passing a boost::tuple instead:
 *
 *    template <int I>
 *    struct functor
 *    {
 *      void operator()(int a);
 *    };
 *
 *    static_for<0, 10, functor>(1);
 *
 * The static_for construct allows for an optional template parameter to change
 * how the loop increments the current iteration value. By default, the inc_once
 * structure above is used.
 *
 * Finally, static_for allows for an optional comparator template parameter, to
 * change how the current loop value is compared to the end loop value. By
 * default, a less operation is performed, to follow the runtime for loop
 * semantics. Alternatively, you can also choose to supply any of the
 * comparators from meta/comparison.h, or your custom constructs.
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
static_for();


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int> class functorT
>
inline void
static_for();


template <
  int START,
  int END,
  template <typename intT, intT, intT> class comparatorT,
  template <int> class functorT
>
inline void
static_for();


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <typename intT, intT, intT> class comparatorT,
  template <int> class functorT
>
inline void
static_for();


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
static_for(functor_paramT & param);


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param);


template <
  int START,
  int END,
  template <typename intT, intT, intT> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param);


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <typename intT, intT, intT> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param);


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
static_for(functor_paramT const & param);


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param);


template <
  int START,
  int END,
  template <typename intT, intT, intT> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param);


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <typename intT, intT, intT> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param);



} // namespace meta

#include <meta/detail/dynamic_for.tcc>
#include <meta/detail/static_for.tcc>

#endif // guard
