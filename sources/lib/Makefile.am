# sources/lib/Makefile.am

SUBDIRS = \
 .

include $(top_srcdir)/build-aux/source_common.mk


AM_LDFLAGS = $(LINKER_FLAGS)
AM_CPPFLAGS = $(COMPILE_FLAGS)

noinst_LTLIBRARIES = \
 libFumaHexdump.la \
 libFumaArena.la

#noinst_HEADERS =

libFumaHexdump_la_SOURCES = \
	hexdump/hexdump.c \
	hexdump/fprint_hex_column.c \
	hexdump/fprint_ascii_column.c

libFumaArena_la_SOURCES = \
	arena/Arena.cpp  \
	arena/GlobalAlloc.cpp  \
	\
	arena/arena_init.c  \
	arena/arena_finit.c \
	arena/arena_alloc_bytes.c \
	arena/arena_free_bytes.c \
	arena/arena_dump_stats.c \
	arena/arena_strdup.c \
	arena/arena_discard.c \
	arena/arena_reclaim.c \
	arena/arena_valid_address.c

libFumaHexdump_la_LIBADD = $(LINKER_LIBS)
libFumaArena_la_LIBADD   = $(LINKER_LIBS)

MAINTAINERCLEANFILES = \
 $(top_srcdir)/sources/lib/Makefile.in
