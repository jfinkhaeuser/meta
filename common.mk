# Always include top-level source directory
AM_CXXFLAGS = -I$(top_srcdir) -I$(top_builddir) @AM_CXXFLAGS@

# Set default compiler flags
AM_CXXFLAGS += --std=c++0x -Wc++0x-compat
AM_CXXFLAGS += \
	-Wall \
	-Wextra \
	-Wabi \
	-Wstrict-null-sentinel \
	-Wold-style-cast
#AM_CXXFLAGS += \
#	-Weffc++

# Dependencies XXX keep these in sync with meta.pc.in
# AM_CXXFLAGS += 
# AM_LDFLAGS += 
