AUTOMAKE_OPTIONS = no-dependencies
DEFS =

AM_CPPFLAGS = -I$(top_srcdir)/lib
if OSLINUX
  LIBS += -lpam_misc
endif

lib_LTLIBRARIES = pam_latch.la
pam_latch_la_SOURCES = pam_latch.c $(top_builddir)/lib/latch.c $(top_builddir)/lib/latch.h $(top_builddir)/lib/util.c \
	$(top_builddir)/lib/util.h $(top_builddir)/lib/drop_privs.c $(top_builddir)/lib/drop_privs.h
pam_latch_la_LDFLAGS = -module -avoid-version
noinst_PROGRAMS = test_pam_latch
test_pam_latch_SOURCES = test.c

pam_latch_LIBS = @LIBS@ -lpam -lcurl -lcrypto -lssl -ldl
test_pam_latch_LIBS = @LIBS@ @linux_test_LIBS@ -lpam


