AUTOMAKE_OPTIONS = no-dependencies
DEFS =

AM_CPPFLAGS = -I$(top_srcdir)/lib
LIBS = @LIBS@ -lcurl -lcrypto -lssl -ldl

bin_PROGRAMS = latch
latch_SOURCES = latch_unix.c latch_unix.h $(top_builddir)/lib/latch.c $(top_builddir)/lib/latch.h $(top_builddir)/lib/util.c \
	$(top_builddir)/lib/util.h $(top_builddir)/lib/drop_privs.c $(top_builddir)/lib/drop_privs.h $(top_builddir)/lib/charset.c \
	$(top_builddir)/lib/charset.h

install-exec-hook:
	echo    "#### Setting SUID for latch ####"
	ls -l $(DESTDIR)$(bindir)/latch
	chmod 4755 $(DESTDIR)$(bindir)/latch
	ls -l $(DESTDIR)$(bindir)/latch
	echo    "####-------------------------------####"
