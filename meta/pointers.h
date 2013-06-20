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

#ifndef META_POINTERS_H
#define META_POINTERS_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

namespace meta {
namespace pointers {

/**
 * The pointer and const_pointer structs simplify writing template code that
 * should work both with pointers and const pointers. It avoids hardcoding the
 * type of pointer to use:
 *
 * template <typename T, template <typename> class pointerT>
 * void foo(pointerT<T>::type bar)
 * {
 *    bar->doSomething();
 * }
 *
 * int const * blargh = ...
 * foo<int, meta::pointers::const_pointer>(blargh);
 *
 * You probably don't need this sort of thing unless you're writing nifty
 * template code like the pointer policies below...
 **/
template <typename T>
struct pointer
{
    typedef T * type;
};

template <typename T>
struct const_pointer
{
    typedef T const * type;
};

template <typename T>
struct const_pointer_const
{
    typedef T const * const type;
};

/**
 * Pointer policies solve a common problem, not by providing particularily
 * clever or complex code, but by giving it a name. There's very little magic
 * here...
 *
 * When handing any sort of object to a class's constructor, and that class
 * keeps a reference or pointer to that object, you always face the same
 * problems:
 *
 *  1) Who owns the object? (that's the obvious problem)
 *  2) What are the semantics for this object, if the enclosing class gets
 *     copy constructed or assigned?
 *
 * Pointer policies answer the second question, by giving different approaches
 * a different name. Some answers to the second question imply how the first
 * question is answered.
 *
 * It's still up to you to decide which approach to use.
 **/
template <
    typename dataT,
    template <typename> class pointerT = const_pointer
>
struct shallow_copy
{
    typename pointerT<dataT>::type m_pointer;

    shallow_copy(typename pointerT<dataT>::type pointer)
        : m_pointer(pointer)
    {
    }


    shallow_copy(shallow_copy const & other)
        : m_pointer(other.m_pointer)
    {
    }


    shallow_copy & operator=(shallow_copy const & other)
    {
        m_pointer = other.m_pointer;
        return *this;
    }


    typename pointerT<dataT>::type operator->() const
    {
        return m_pointer;
    }
};


template <
    typename dataT,
    template <typename> class pointerT = const_pointer
>
struct deep_copy
{
    typename pointerT<dataT>::type m_pointer;

    /**
     * Must distinguish between copy-constructed and non-copy-constructed
     * instances of deep_copy - copy-constructed instances must take ownership
     * of m_pointer, because they are allocated within one of the deep_copy
     * functions.
     **/
    bool m_owner;

    deep_copy(typename pointerT<dataT>::type pointer)
        : m_pointer(pointer)
        , m_owner(false)
    {
    }


    deep_copy(deep_copy const & other)
        : m_pointer(new dataT(*other.m_pointer))
        , m_owner(true)
    {
    }


    ~deep_copy()
    {
        if (m_owner) {
            delete m_pointer;
        }
    }


    deep_copy & operator=(deep_copy const & other)
    {
        if (m_owner) {
            delete m_pointer;
        }
        m_pointer = new dataT(*other.m_pointer);
        m_owner = true;
        return *this;
    }


    typename pointerT<dataT>::type operator->() const
    {
        return m_pointer;
    }
};



}} // namespace meta::pointers

#endif // guard
