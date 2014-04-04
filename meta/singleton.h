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
#ifndef META_SINGLETON_H
#define META_SINGLETON_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <meta/meta-config.h>

#if META_CXX_MODE != META_CXX_MODE_CXX0X
#error Can't compile meta/singleton.h because there's no C++11 support.
#endif

#include <utility>

#include <meta/noncopyable.h>

namespace meta {

namespace detail {
/**
 * The default lock policy for the singleton does nothing. A lock policy you
 * provide needs to:
 *  - Be default-constructible (and therefore destructible)
 *  - provide a lock() function
 *  - provide an unlock() function
 *  - provide a memory_barrier() function.
 *
 * The last requirement *may* be dropped on C++11 systems (assuming the C++11
 * memory model is sufficiently thread-aware).
 **/
struct default_lock_policy
{
  inline void lock() {}
  inline void unlock() {}
  inline void memory_barrier() {}
};


/**
 * The default destruction policy is to register the passed callback via the
 * standard atexit() function. For testing, or for ordered destruction of
 * singletons, it may be worthwhile implementing your own.
 **/
struct default_destruction_policy
{
  static inline void register_destruction_callback(void (*func)(void))
  {
    atexit(func);
  }
};


/**
 * Helper scope guard using lock policies.
 **/
template <typename T>
struct scope_guard
{
  T & policy;

  scope_guard(T & _policy)
    : policy(_policy)
  {
    policy.lock();
  }

  ~scope_guard()
  {
    policy.unlock();
  }
};

} // namespace detail

/**
 * Thread-safe singleton implementation, provided the provided thread policy
 * works as intended.
 *
 * Us
 **/
template <
  typename T,
  typename lock_policyT = detail::default_lock_policy,
  typename destruction_policyT = detail::default_destruction_policy
>
class singleton
{
public:
  virtual ~singleton()
  {
    // Signal that this object is dead, but don't destroy things yet!
    sm_instance_pointer = nullptr;
    sm_destroyed = true;
  }



  template <typename... Args>
  static inline T const & const_instance(Args && ... args)
  {
    return *const_cast<T*>(get_instance(std::forward<Args>(args)...));
  }



  template <typename... Args>
  static inline T & mutable_instance(Args && ... args)
  {
    return *const_cast<T*>(get_instance(std::forward<Args>(args)...));
  }


private:
  template <typename... Args>
  static inline volatile T * get_instance(Args && ... args)
  {
    // Barrier before reading sm_instance_pointer
    sm_lock_policy.memory_barrier();
    if (!sm_instance_pointer) {

      // Only create lock if there does not appear to be an instance
      detail::scope_guard<lock_policyT> guard(sm_lock_policy);

      // Barrier before reading sm_instance_pointer
      sm_lock_policy.memory_barrier();
      if (!sm_instance_pointer) {
        volatile T * tmp = new T(std::forward<Args>(args)...);
        sm_instance_pointer = tmp;

        // Register function for destroying the singleton
        destruction_policyT::register_destruction_callback(destroy_instance);
      }
    }
    return sm_instance_pointer;
  }


  static void destroy_instance()
  {
    detail::scope_guard<lock_policyT> guard(sm_lock_policy);
    sm_lock_policy.memory_barrier();
    sm_instance_pointer->~T();
  }


  // Need to instanciate lock policy in case it has state (such as a mutex)
  static lock_policyT   sm_lock_policy;
  static volatile bool  sm_destroyed;
  static volatile T *   sm_instance_pointer;
};


template <typename T, typename lock_policyT, typename destruction_policyT>
lock_policyT
singleton<T, lock_policyT, destruction_policyT>::sm_lock_policy = lock_policyT();

template <typename T, typename lock_policyT, typename destruction_policyT>
volatile bool
singleton<T, lock_policyT, destruction_policyT>::sm_destroyed = false;


template <typename T, typename lock_policyT, typename destruction_policyT>
volatile T *
singleton<T, lock_policyT, destruction_policyT>::sm_instance_pointer = nullptr;


} // namespace meta

#endif // guard
