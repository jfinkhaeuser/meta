cmake_minimum_required(VERSION 2.8)
project(meta)

set (META_PACKAGE_MAJOR 0)
set (META_PACKAGE_MINOR 1)
set (META_PACKAGE_URL https://github.com/unwesen/meta)

include(CheckIncludeFiles)
check_include_files(endian.h META_HAVE_ENDIAN_H)
check_include_files(byteswap.h META_HAVE_BYTESWAP_H)

include(FindPkgConfig)
pkg_check_modules(CPPUNIT cppunit>=1.12)

configure_file(
    "${PROJECT_SOURCE_DIR}/meta/meta-config.h.in"
    "${PROJECT_BINARY_DIR}/meta/meta-config.h"
)

if (CMAKE_COMPILER_IS_GNUCXX)
  set (CMAKE_CXX_FLAGS "-std=c++0x -Wc++0x-compat")
  set (CMAKE_CXX_FLAGS_DEBUG "-std=c++0x -Wc++0x-compat")
  set (CMAKE_CXX_FLAGS_RELEASE "-std=c++0x -Wc++0x-compat")
endif ()


include_directories(
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}"
)

install(FILES
    meta/meta-config.h
    meta/meta.h
    meta/version.h
    meta/for.h
    meta/comparison.h
    meta/noncopyable.h
    meta/mandatory.h
    meta/byteorder.h
    meta/pointers.h
    meta/nullptr.h
    meta/condition.h
    meta/typelist.h
    meta/restricted.h
    meta/restrictions.h
    meta/singleton.h
    meta/math.h
    meta/hash.h
    meta/range.h
    DESTINATION include/meta)

install(FILES
    meta/detail/dynamic_for.tcc
    meta/detail/static_for.tcc
    meta/detail/dual_operators.tcc
    meta/detail/chain_helper.tcc
    meta/detail/dynamic.tcc
    meta/detail/typelist.h
    meta/detail/inhlist.h
    meta/detail/complist.h
    meta/detail/list_revert.h
    meta/detail/list_prepend_unique.h
    meta/detail/list_contains.h
    meta/detail/list_append_unique.h
    meta/detail/list_append.h
    meta/detail/list_prepend.h
    meta/detail/list_make_unique.h
    DESTINATION include/meta/detail)

install(FILES
    meta/restrictions/numeric.h
    meta/restrictions/container.h
    DESTINATION include/meta/restrictions)

if (CPPUNIT_FOUND)
  add_executable(testsuite
      test/test_mandatory.cpp
      test/test_meta.cpp
      test/test_byteorder.cpp
      test/test_pointers.cpp
      test/test_math.cpp
      test/test_hash.cpp
      test/test_condition.cpp
      test/test_restricted.cpp
      test/test_singleton.cpp
      test/test_lists.cpp
      test/test_range.cpp
      test/testsuite.cpp)
  target_link_libraries(testsuite ${DEP_LIBRARIES} ${CPPUNIT_LDFLAGS})

  add_test(NAME "Testsuite" COMMAND testsuite)
endif ()
