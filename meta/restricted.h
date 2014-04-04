/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.co.uk>
 *
 * Copyright (c) 2014 Unwesen Ltd.
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

#ifndef META_RESTRICTED_H
#define META_RESTRICTED_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta.h>

#if META_CXX_MODE != META_CXX_MODE_CXX0X
#error Can't compile meta/restricted.h because there's no C++11 support.
#endif

#include <stdexcept>

#include <meta/condition.h>


namespace meta {
namespace restrictions {

/**
 * The class throws violation_error if it's to be assigned a value that did
 * not pass the restriction chain.
 **/
typedef std::runtime_error violation_error;

} // namespace restrictions




/**
 * The restricted template class simulates a regular value type (as specified by
 * the WrappedType template parameter), but restricts the range of values you may
 * assign to it. If an invalid value is being assigned, an exception is thrown.
 *
 * The purpose of this class is to provide something similar to contract checks
 * in other languages, i.e. to enforce that the parameters to a function conform
 * to a certain pattern, as does the return value:
 *
 *    restricted<int, range<int, 0, 20> >
 *    foo(restricted<std::string, restricted<std::string, non_empty> const & param)
 *    {
 *        // return 21; // would throw
 *        return 10; // doesn't throw
 *    }
 *
 *    foo(""); // would throw
 *
 * Of course, these type definitions are pretty ugly; it's probably best to
 * typedef them. See <meta/restrictions.h> for some predefined restrictions,
 * but you can also roll your own.
 *
 * Restrictions are conditions as defined by the <meta/condition.h> header, and
 * they're passed the value to be assigned. You can provide complex restrictions
 * assembled via the condition's _and and _or constructs, or you can pass multiple
 * restrictions after the value type to the restricted template class. Multiple
 * restrictions are treated as if they were chained together with _and.
 *
 * It's possible to not pass any conditions at all - that means the restricted
 * type does not enforce anything.
 **/
template <
  /** The type to wrap */
  typename WrappedType,
  /** The restriction chain to apply */
  typename... Conditions
>
class restricted
{
public:
  /** Convenience */
  typedef restricted<WrappedType, Conditions...> own_type;

  /**
   * This constructor is the default ctor, and the only copy ctor restricted
   * allows - if you want to copy from one restricted to another, the cast
   * operator to WrappedType will be invoked automatically.
   **/
  restricted(WrappedType const & value = WrappedType())
    : m_value(check(value))
  {
  }

  // FIXME restricted(WrappedType const & value = WrappedType())
  // FIXME   : m_value(check(value))
  // FIXME {
  // FIXME }


  /**
   * An explicit copy/move constructor for restricted is required to make
   * restricted work in a return value specification. However, checking the
   * value passed in is unnecessary, as the 'other' restricted must already
   * have passed these checks during it's construction...
   **/
  restricted(restricted && other)
    : m_value(other.m_value)
  {
  }

  /**
   * For similar reasons, and explicit assignment operator is a good idea.
   **/
  restricted & operator=(restricted const & other)
  {
    m_value = other.m_value;
    return *this;
  }


  /** Assignment operators */
  template <typename otherT>
  own_type & operator=(otherT const & other)
  {
    m_value = check(other);
    return *this;
  }

  template <typename otherT>
  own_type & operator*=(otherT const & other)
  {
    WrappedType tmp = m_value;
    tmp *= other;
    m_value = check(tmp);
    return *this;
  }

  template <typename otherT>
  own_type & operator/=(otherT const & other)
  {
    WrappedType tmp = m_value;
    tmp /= other;
    m_value = check(tmp);
    return *this;
  }

  template <typename otherT>
  own_type & operator%=(otherT const & other)
  {
    WrappedType tmp = m_value;
    tmp %= other;
    m_value = check(tmp);
    return *this;
  }

  template <typename otherT>
  own_type & operator+=(otherT const & other)
  {
    WrappedType tmp = m_value;
    tmp += other;
    m_value = check(tmp);
    return *this;
  }

  template <typename otherT>
  own_type & operator-=(otherT const & other)
  {
    WrappedType tmp = m_value;
    tmp -= other;
    m_value = check(tmp);
    return *this;
  }

  template <typename otherT>
  own_type & operator<<=(otherT const & other)
  {
    WrappedType tmp = m_value;
    tmp <<= other;
    m_value = check(tmp);
    return *this;
  }

  template <typename otherT>
  own_type & operator>>=(otherT const & other)
  {
    WrappedType tmp = m_value;
    tmp >>= other;
    m_value = check(tmp);
    return *this;
  }

  template <typename otherT>
  own_type & operator&=(otherT const & other)
  {
    WrappedType tmp = m_value;
    tmp &= other;
    m_value = check(tmp);
    return *this;
  }

  template <typename otherT>
  own_type & operator^=(otherT const & other)
  {
    WrappedType tmp = m_value;
    tmp ^= other;
    m_value = check(tmp);
    return *this;
  }

  template <typename otherT>
  own_type & operator|=(otherT const & other)
  {
    WrappedType tmp = m_value;
    tmp |= other;
    m_value = check(tmp);
    return *this;
  }

  /** Arithmetic operators */

  // prefix operator ++
  own_type & operator++()
  {
    WrappedType tmp = m_value;
    ++tmp;
    m_value = check(tmp);
    return *this;
  }

  // prefix operator --
  own_type & operator--()
  {
    WrappedType tmp = m_value;
    --tmp;
    m_value = check(tmp);
    return *this;
  }


  /** Casting operator for the wrapped type. */
  operator WrappedType() const
  {
    return m_value;
  }

  /** Utility functions */
  void swap(own_type & other)
  {
    // Both this object and other must have passed restrictions to set their
    // m_value. And the restrictions are identical. So we can just swap
    // m_value.
    std::swap(m_value, other.m_value);
  }

private:
  /** Check function for the value */
  inline WrappedType const & check(WrappedType const & new_value) const
  {
    if (::meta::condition::_and<Conditions...>::check(new_value)) {
      return new_value;
    }
    throw ::meta::restrictions::violation_error("Assignment violates restricted conditions.");
    return m_value; // Not reached, but keeps compiler quiet
  }

  /** wrapped value */
  WrappedType m_value;
};


} // namespace meta


namespace std {

// Specialize std::swap for restricted types.
template <
  typename WrappedType,
  typename... Conditions
>
inline void swap(meta::restricted<WrappedType, Conditions...> & first,
  meta::restricted<WrappedType, Conditions...> & second)
{
  first.swap(second);
}

} // namespace std


#endif // guard
