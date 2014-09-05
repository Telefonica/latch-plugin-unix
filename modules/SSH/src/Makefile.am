AUTOMAKE_OPTIONS = no-dependencies
DEFS =

LIBS = @LIBS@ -lcurl -lcrypto -lssl -ldl

bin_PROGRAMS = latch-shell
latch_shell_SOURCES = 	latch_shell.c \
			$(top_builddir)/lib/latch.c \
			$(top_builddir)/lib/util.c \
			$(top_builddir)/lib/latch.h \
			$(top_builddir)/lib/util.h

install-exec-hook:
	echo    "#### Setting SUID for latch-shell ####"
	ls -l $(DESTDIR)$(bindir)/latch-shell
	chmod 4755 $(DESTDIR)$(bindir)/latch-shell
	ls -l $(DESTDIR)$(bindir)/latch-shell
	echo    "####-------------------------------####"
