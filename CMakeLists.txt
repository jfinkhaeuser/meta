##############################################################################
# Project metadata
cmake_minimum_required(VERSION 2.8)

project(meta)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

set (META_PACKAGE_MAJOR 1)
set (META_PACKAGE_MINOR 0)
set (META_PACKAGE_URL https://github.com/jfinkhaeuser/meta)

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
find_package(cppunit 1.12.1)

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
  target_link_libraries(testsuite ${DEP_LIBRARIES} ${CPPUNIT_LIBRARIES})

  # Since some time after 2.8.9
  if ("${CMAKE_VERSION}" VERSION_GREATER "2.8.10")
    target_include_directories(testsuite PRIVATE ${CPPUNIT_INCLUDE_DIR})
  else()
    include_directories(testsuite PRIVATE ${CPPUNIT_INCLUDE_DIR})
  endif()

  set_target_properties(testsuite PROPERTIES LINK_FLAGS "${TESTSUITE_LINK_FLAGS}")

  add_test(NAME "Testsuite" COMMAND testsuite)
  set(PROJECT_TEST_NAME testsuite)

  # Code coverage
  if(CMAKE_COMPILER_IS_GNUCXX)
    include(CodeCoverage)
    setup_target_for_coverage(coverage ${PROJECT_TEST_NAME} coverage)
    target_link_libraries(${PROJECT_TEST_NAME} gcov)
  endif()
endif (CPPUNIT_FOUND)

##############################################################################
# CPack Section
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ Metaprogramming Primivites and Pattern Implementations")
set(CPACK_PACKAGE_VENDOR "finkhaeuser consulting")
set(CPACK_PACKAGE_CONTACT "jens@finkhaeuser.de")
set(CPACK_PACKAGE_VERSION_MAJOR "${META_PACKAGE_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${META_PACKAGE_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
# set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

if(WIN32 AND NOT UNIX)
  set(CPACK_GENERATOR "NSIS;NSIS64")
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  set(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}\\\\data\\\\tool-icon.bmp")
  # set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} meta")
  set(CPACK_NSIS_HELP_LINK "http://jfinkhaeuser.github.io/meta/")
  set(CPACK_NSIS_URL_INFO_ABOUT "http://jfinkhaeuser.github.io/meta/")
  set(CPACK_NSIS_CONTACT "jens@finkhaeuser.de")
  set(CPACK_NSIS_MODIFY_PATH ON)
else(WIN32 AND NOT UNIX)
  set(CPACK_GENERATOR "STGZ;TGZ;TBZ2;DEB")
  # set(CPACK_STRIP_FILES "bin/MyExecutable")
  set(CPACK_SOURCE_STRIP_FILES "")

  # Debian
  # set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libgcc1 (>= 1:3.4.2-12)")
endif(WIN32 AND NOT UNIX)

# set(CPACK_PACKAGE_EXECUTABLES "MyExecutable" "My Executable")

include(CPack)
