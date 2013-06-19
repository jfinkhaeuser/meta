# Always include top-level source directory
AM_CXXFLAGS = -I$(top_srcdir) -I$(top_builddir) @AM_CXXFLAGS@

# Set default compiler flags
AM_CXXFLAGS += -g --std=c++0x

# Dependencies XXX keep these in sync with meta.pc.in
# AM_CXXFLAGS += 
# AM_LDFLAGS += 
