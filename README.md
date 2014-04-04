meta
====

C++ Metaprogramming Primivites and Pattern implementations.

[![Build Status](https://travis-ci.org/jfinkhaeuser/meta.svg?branch=master)](https://travis-ci.org/jfinkhaeuser/meta)

This library is kept deliberately small, with no external dependencies (1).

- `for.h` provides compile-time for loops
- `comparison.h` provides compile-time comparators
- `noncopyable.h` provides a non-copyable base class
- `mandatory.h` provides code for enforcing checking of return types
- `byteorder.h` provides compiler-/platform-independent versions of ntoh/hton
  for integer types (including 64 bits)
- `pointers.h` provides policies for shallow-copying/deep-copying pointer
  members.
- `typelist.h` provides constructs for constructing and manipulating lists of
  types.
  - `typelist` is a list of types with no value to itself.
  - `inheritancelist` inherits from all types and therefore does not work with
    POD types.
  - `compositionlist` composes all types and therefore does work with POD types.
- `condition.h` types for compile-time composition of condition chains with
  conditions being either stateless or stateful.

Some of these bits of code started out as mental exercises, so there's no
guarantee for usefulness implied in providing them. See COPYING for details.

1. There are external dependencies, but only for testing:
  - `CppUnit` for the unit test framework.
  - `boost::posix_time` for tests involving timing.


C++ Compatibility
-----------------

This library is mostly written with `c++0x` features in mind, but some of it's
code can also compile in `c++98`. A few headers can be compiled in either mode.

To force `meta`'s configure script into building only `c++98` compatible tests,
invoke it like this:

```bash
$ CXXFLAGS=-DMETA_CXX_MODE=META_CXX_MODE_CXX98 ./configure
```

Similarly, if you're including meta headers into your `c++98` project, make
sure to set the same define - either before the include statement, or on the
command line.
