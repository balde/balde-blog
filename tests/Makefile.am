SUBDIRS = foo

check_PROGRAMS = \
	check_post_loader \
	check_post_parser

TESTS = \
	$(check_PROGRAMS)

check_post_loader_SOURCES = \
	check_post_loader.c

check_post_parser_SOURCES = \
	check_post_parser.c

AM_CFLAGS = \
	$(all_includes) \
	$(GLIB_CFLAGS) \
	$(BALDE_CFLAGS) \
	$(BALDE_MARKDOWN_CFLAGS) \
	-I$(top_srcdir)/include

AM_LDFLAGS = \
	$(GLIB_LIBS) \
	$(BALDE_LIBS) \
	$(BALDE_MARKDOWN_LIBS) \
	$(top_builddir)/libbalde_blog/libbalde_blog.la

if USE_VALGRIND
valgrind:
	$(MAKE) check TESTS_ENVIRONMENT=" \
		G_SLICE=always-malloc \
		G_DEBUG=gc-friendly \
		$(LIBTOOL) \
			--mode=execute \
			$(VALGRIND) \
				--tool=memcheck \
				--leak-check=full \
				--leak-resolution=high \
				--num-callers=20 \
				--show-possibly-lost=no"
endif
