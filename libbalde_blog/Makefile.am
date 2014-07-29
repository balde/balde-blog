lib_LTLIBRARIES = libbalde_blog.la

libbalde_blog_la_SOURCES =

libbalde_blog_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(BALDE_CFLAGS) \
	$(BALDE_MARKDOWN_CFLAGS) \
	-I$(top_srcdir)/include

libbalde_blog_la_LDFLAGS = \
	$(AM_LDADD) \
	$(GLIB_LIBS) \
	$(BALDE_LIBS) \
	$(BALDE_MARKDOWN_LIBS) \
	-version-info $(LT_VERSION_INFO)
