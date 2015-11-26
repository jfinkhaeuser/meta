##############################################################################
# Project metadata
cmake_minimum_required(VERSION 2.8)
project(meta)

set (META_PACKAGE_MAJOR 0)
set (META_PACKAGE_MINOR 1)
set (META_PACKAGE_URL https://github.com/unwesen/meta)

##############################################################################
# Options
option(META_USE_CXX11
    "Use C++11 features; this will build more tests." ON)

set (META_CXX_MODE_CXX0X 0x2011)
set (META_CXX_MODE_CXX98 0x1998)

if (META_USE_CXX11)
  set (META_CXX_MODE_BUILD ${META_CXX_MODE_CXX0X})
else (META_USE_CXX11)
  set (META_CXX_MODE_BUILD ${META_CXX_MODE_CXX98})
endif (META_USE_CXX11)

##############################################################################
# Compiler flags #1
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message("Selecting compiler flags for clang++")

  set (CLANGXX_CXX_FLAGS "-Wall -Wextra -Wabi -Wold-style-cast"
      CACHE STRING "Default flags for clang++")
  if (META_USE_CXX11)
    set (CLANGXX_CXX_FLAGS_STD "-std=c++0x -Wc++0x-compat")
  else (META_USE_CXX11)
    set (CLANGXX_CXX_FLAGS_STD "-std=c++98")
  endif (META_USE_CXX11)
  set (CMAKE_CXX_FLAGS "${CLANGXX_CXX_FLAGS} ${CLANGXX_CXX_FLAGS_STD}")
  set (CMAKE_CXX_FLAGS_DEBUG "${CLANGXX_CXX_FLAGS} ${CLANGXX_CXX_FLAGS_STD} -O0 -ggdb")
  set (CMAKE_CXX_FLAGS_RELEASE "${CLANGXX_CXX_FLAGS} ${CLANGXX_CXX_FLAGS_STD} -O3 -ggdb")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message("Selecting compiler flags for GNU c++")

  set (GNUCXX_CXX_FLAGS "-Wall -Wextra -Wold-style-cast -Wstrict-null-sentinel"
      CACHE STRING "Default flags for G++")
  if (META_USE_CXX11)
    set (GNUCXX_CXX_FLAGS_STD "-std=c++0x -Wc++0x-compat")
  else (META_USE_CXX11)
    set (GNUCXX_CXX_FLAGS_STD "-std=c++98")
  endif (META_USE_CXX11)
  set (CMAKE_CXX_FLAGS "${GNUCXX_CXX_FLAGS} ${GNUCXX_CXX_FLAGS_STD}")
  set (CMAKE_CXX_FLAGS_DEBUG "${GNUCXX_CXX_FLAGS} ${GNUCXX_CXX_FLAGS_STD} -O0 -ggdb")
  set (CMAKE_CXX_FLAGS_RELEASE "${GNUCXX_CXX_FLAGS} ${GNUCXX_CXX_FLAGS_STD} -O3 -ggdb")
endif ()

##############################################################################
# Headers
include(CheckIncludeFileCXX)
check_include_file_cxx(endian.h META_HAVE_ENDIAN_H)
check_include_file_cxx(byteswap.h META_HAVE_BYTESWAP_H)
check_include_file_cxx(cstdint META_HAVE_STD_HEADERS)
check_include_file_cxx(tr1/cstdint META_HAVE_TR1_HEADERS)


##############################################################################
# Dependencies
include(FindPkgConfig)
pkg_check_modules(CPPUNIT cppunit>=1.12)


##############################################################################
# Platform checks
include(TestBigEndian)
test_big_endian(META_BIGENDIAN)


##############################################################################
# Write configuration
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

configure_file(
    "${PROJECT_SOURCE_DIR}/meta/meta-config.h.in"
    "${PROJECT_BINARY_DIR}/meta/meta-config.h"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/meta.pc.in"
    "${PROJECT_BINARY_DIR}/meta.pc"
    @ONLY
)


##############################################################################
# Compiler flags #2

# Add include paths
include_directories(
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}"
)

# Add library flags
set (CMAKE_CXX_FLAGS          "${CPPUNIT_CFLAGS} ${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS_DEBUG    "${CPPUNIT_CFLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
set (CMAKE_CXX_FLAGS_RELEASE  "${CPPUNIT_CFLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")


##############################################################################
# Build artefacts
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/meta/meta-config.h
    meta/meta.h
    meta/version.h
    meta/inttypes.h
    meta/for.h
    meta/comparison.h
    meta/noncopyable.h
    meta/stackonly.h
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

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/meta.pc
    DESTINATION lib/pkgconfig)


##############################################################################
# Tests
if (CPPUNIT_FOUND)
  # Tests compatible with all C++ versions
  set(TEST_SOURCES
      test/timing.cpp
      test/test_mandatory.cpp
      test/test_meta.cpp
      test/test_byteorder.cpp
      test/test_pointers.cpp
      test/test_math.cpp
  )

  # Tests compatible with C++11 only
  if (META_USE_CXX11)
    set(TEST_SOURCES ${TEST_SOURCES}
      test/test_hash.cpp
      test/test_condition.cpp
      test/test_restricted.cpp
      test/test_singleton.cpp
      test/test_lists.cpp
      test/test_range.cpp
    )
  endif (META_USE_CXX11)

  add_executable(testsuite
      ${TEST_SOURCES}
      test/testsuite.cpp)
  target_link_libraries(testsuite ${DEP_LIBRARIES} ${CPPUNIT_LDFLAGS})

  add_test(NAME "Testsuite" COMMAND testsuite)
endif (CPPUNIT_FOUND)
