AUTOMAKE_OPTIONS = no-dependencies

DEFS =

if OSLINUX
  LIBS += -lpam_misc
endif

lib_LTLIBRARIES = pam_latch.la
pam_latch_la_SOURCES = pam_latch.c ../lib/latch.c ../lib/latch.h ../lib/util.c ../lib/util.h
pam_latch_la_LDFLAGS = -module -avoid-version
bin_PROGRAMS = test
test_SOURCES = test.c

pam_latch_LIBS = @LIBS@ -lpam -lcurl -lcrypto -lssl -ldl

test_LIBS = @LIBS@ @linux_test_LIBS@ -lpam


