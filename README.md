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
- `math.h` for some simple compile-time mathematics, such as keeping a ratio of
  two numbers and converting into another ratio, etc.
- `nullptr.h` for `nullptr` support in compilers that don't know it yet.
- `singleton.h` for a simple singleton implementation.
- `restricted.h` and `restrictions.h` for types that allow only certain ranges
  of values to be assigned to them; you can use them in a function prototype
  to enforce a contract, for example.

Some of these bits of code started out as mental exercises, so there's no
guarantee for usefulness implied in providing them. See COPYING for details.


Requirements
------------

There are external dependencies, but only for testing:
- [CppUnit](cppunit.sourceforge.net) for the unit test framework.
- `boost::posix_time` for tests involving timing.


Installation
------------

After installing the requirements, run:

```bash
$ autoreconf -siv
```

This will install missing build files and create a `configure` script. Usually, you can just run the
following commands to get going:

```bash
$ ./configure && make check
```

For more details, check out the `INSTALL` file that was being generated/linked in the first step.


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
