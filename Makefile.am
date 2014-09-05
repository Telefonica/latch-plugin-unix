ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src pam modules/SSH/src


EXTRA_DIST = AUTHORS ChangeLog NEWS README.md


install-data-local:
	$(MKDIR_P) $(DESTDIR)$(sysconfdir)
	-@if [ ! -f $(DESTDIR)$(sysconfdir)/latch.conf ]; then \
		cp examples/latch/latch.conf $(DESTDIR)$(sysconfdir)/latch.conf; \
		echo "Created ${DESTDIR}$(sysconfdir)/latch.conf"; \
		echo "Please edit it to add your Application Id and your Secret Key"; \
	else \
		echo "Found existing ${DESTDIR}$(sysconfdir)/latch.conf - updating permissions"; \
	fi
	-chmod 600 $(DESTDIR)$(sysconfdir)/latch.conf
	-@if [ ! -f $(DESTDIR)$(sysconfdir)/latch.accounts ]; then \
		cp examples/latch/latch.accounts $(DESTDIR)$(sysconfdir)/latch.accounts; \
		echo "Created ${DESTDIR}$(sysconfdir)/latch.accounts"; \
	else \
		echo "Found existing ${DESTDIR}$(sysconfdir)/latch.accounts - updating permissions"; \
	fi
	-chmod 600 $(DESTDIR)$(sysconfdir)/latch.accounts


uninstall-local:
	-@if [ -d $(DESTDIR)$(sysconfdir) ]; then \
		echo "Removing latch conf directory..."; \
		rm -R $(DESTDIR)$(sysconfdir); \
	fi

