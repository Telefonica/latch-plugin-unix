AUTOMAKE_OPTIONS = no-dependencies
DEFS =

AM_CPPFLAGS = -I$(top_srcdir)/lib
LIBS = @LIBS@ -lcurl -lcrypto -lssl -ldl

bin_PROGRAMS = latch-ssh-cmd
latch_ssh_cmd_SOURCES = 	latch_ssh_command.c \
			$(top_builddir)/lib/latch.c \
			$(top_builddir)/lib/latch.h \
			$(top_builddir)/lib/util.c \
			$(top_builddir)/lib/util.h \
			$(top_builddir)/lib/drop_privs.c \
			$(top_builddir)/lib/drop_privs.h

install-exec-hook:
	echo    "#### Setting SUID for latch-ssh-cmd ####"
	ls -l $(DESTDIR)$(bindir)/latch-ssh-cmd
	chmod 4755 $(DESTDIR)$(bindir)/latch-ssh-cmd
	ls -l $(DESTDIR)$(bindir)/latch-ssh-cmd
	echo    "####-------------------------------####"
