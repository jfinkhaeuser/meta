# Always include top-level source directory
AM_CXXFLAGS = -I$(top_srcdir) -I$(top_builddir) @AM_CXXFLAGS@

# Set default compiler flags
if CXX_MODE_CXX0X
AM_CXXFLAGS += -std=c++0x -Wc++0x-compat
elif CXX_MODE_CXX98
AM_CXXFLAGS += -std=c++98
endif

AM_CXXFLAGS += \
	-Wall \
	-Wextra \
	-Wabi \
	-Wold-style-cast

if HAVE_FLAG__WSTRICT_NULL_SENTINEL
AM_CXXFLAGS += \
	-Wstrict-null-sentinel
endif

# Dependencies XXX keep these in sync with meta.pc.in
# AM_LDFLAGS += 
