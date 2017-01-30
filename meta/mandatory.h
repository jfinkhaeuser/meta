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

#ifndef META_MANDATORY_H
#define META_MANDATORY_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

#include <stdexcept>

#include <meta/noncopyable.h>

#if META_CXX_MODE == META_CXX_MODE_CXX0X
#  define META_DTOR_THROW noexcept(false)
#else
#  define META_DTOR_THROW throw(std::logic_error)
#endif

namespace meta {

/**
 * An implementation of Andrei Alexandrescu's http://www.ddj.com/dept/cpp/184401917
 *
 * The throw_if_unchecked class template can be used to wrap around return
 * types of functions that you want the caller to examine:
 *
 *    throw_if_unchecked<int> foo() {...}
 *
 *    foo();                                    // throws
 *    throw_if_unchecked<int> ret = foo();      // throws
 *    int ret = foo();                          // doesn't throw, value is 'used'
 *    static_cast<ignore_return_value>(foo());  // doesn't throw, discarded
 *    ignore_return_value x = foo();            // doesn't throw, discarded
 *
 * The error thrown in the cases marked above are std::logic_error.
 **/

struct ignore_return_value {};

template <
    // the wrapped type
    typename wrappedT
>
class throw_if_unchecked
{
public:
    inline throw_if_unchecked(wrappedT const & retval)
        : m_throw(true)
        , m_retval(retval)
    {
    }

    inline throw_if_unchecked(throw_if_unchecked const & other)
        : m_throw(other.m_throw)
        , m_retval(other.m_retval)
    {
    }

#if META_CXX_MODE == META_CXX_MODE_CXX0X
    inline throw_if_unchecked(throw_if_unchecked && other)
      : m_throw(other.m_throw)
      , m_retval(other.m_retval)
    {
      other.m_throw = false;
      other.m_retval = wrappedT();
    }
#endif

    inline ~throw_if_unchecked() META_DTOR_THROW
    {
        if (m_throw) {
            throw std::logic_error("Ignored return value that must not be ignored!");
        }
    }

    inline operator wrappedT() const
    {
        m_throw = false;
        return m_retval;
    }

    inline operator ignore_return_value() const
    {
        m_throw = false;
        return ignore_return_value();
    }

private:
    // No default construction
    throw_if_unchecked();

    mutable bool m_throw;
    wrappedT     m_retval;
};


} // namespace meta

#endif // guard
