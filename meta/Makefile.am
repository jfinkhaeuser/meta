include $(top_srcdir)/common.mk

SUBDIRS = .

include_metadir = $(includedir)/meta
include_meta_HEADERS = \
	meta-config.h \
	version.h \
	for.h \
	comparison.h \
	noncopyable.h \
	mandatory.h \
	byteorder.h \
	pointers.h \
	nullptr.h \
	condition.h \
	typelist.h \
	restricted.h \
	restrictions.h \
	singleton.h \
	math.h

include_detaildir = $(include_metadir)/detail
include_detail_HEADERS = \
	detail/dynamic_for.tcc \
	detail/static_for.tcc \
	detail/dual_operators.tcc \
	detail/chain_helper.tcc \
	detail/dynamic.tcc \
	detail/typelist.h \
	detail/inhlist.h \
	detail/complist.h \
	detail/list_revert.h \
	detail/list_prepend_unique.h \
	detail/list_contains.h \
	detail/list_append_unique.h \
	detail/list_append.h \
	detail/list_prepend.h \
	detail/list_make_unique.h

include_restrictionsdir = $(include_metadir)/restrictions
include_restrictions_HEADERS = \
	restrictions/numeric.h \
	restrictions/container.h
