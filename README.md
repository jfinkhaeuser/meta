meta
====

C++ Metaprogramming Primivites and Pattern implementations.

This library is kept deliberately small, with no external dependencies (other
than CppUnit for testing).

- for.h provides compile-time for loops
- comparison.h provides compile-time comparators
- noncopyable.h provides a non-copyable base class
- mandatory.h provides code for enforcing checking of return types
- byteorder.h provides compiler-/platform-independent versions of ntoh/hton for
  integer types (including 64 bits)

Some of this code started out as a mental exercise, so there's no guarantee for
usefulness implied in providing them.
