AUTOMAKE_OPTIONS = no-dependencies
DEFS =

LIBS = @LIBS@ -lcurl -lcrypto -lssl -ldl

bin_PROGRAMS = latch
latch_SOURCES = latch_unix.c latch_unix.h ../lib/latch.c ../lib/latch.h ../lib/util.c ../lib/util.h \
                ../lib/drop_privs.c ../lib/drop_privs.h ../lib/charset.c ../lib/charset.h

install-exec-hook:
	echo    "#### Setting SUID for latch ####"
	ls -l $(DESTDIR)$(bindir)/latch
	chmod 4755 $(DESTDIR)$(bindir)/latch
	ls -l $(DESTDIR)$(bindir)/latch
	echo    "####-------------------------------####"
