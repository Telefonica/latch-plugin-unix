ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src pam modules/SSH/src


EXTRA_DIST = ChangeLog README.md


install-data-local:
	$(MKDIR_P) $(DESTDIR)$(sysconfdir)/latch
	-@if [ ! -f $(DESTDIR)$(sysconfdir)/latch/latch.conf ]; then \
		cp examples/latch/latch.conf $(DESTDIR)$(sysconfdir)/latch/latch.conf; \
		echo "Created ${DESTDIR}$(sysconfdir)/latch/latch.conf"; \
		echo "Please edit it to add your Application Id and your Secret Key"; \
	else \
		echo "Found existing ${DESTDIR}$(sysconfdir)/latch/latch.conf - updating permissions"; \
	fi
	-chmod 600 $(DESTDIR)$(sysconfdir)/latch/latch.conf
	-@if [ ! -f $(DESTDIR)$(sysconfdir)/latch/latch.accounts ]; then \
		cp examples/latch/latch.accounts $(DESTDIR)$(sysconfdir)/latch/latch.accounts; \
		echo "Created ${DESTDIR}$(sysconfdir)/latch/latch.accounts"; \
	else \
		echo "Found existing ${DESTDIR}$(sysconfdir)/latch/latch.accounts - updating permissions"; \
	fi
	-chmod 600 $(DESTDIR)$(sysconfdir)/latch/latch.accounts


uninstall-local:
	-@if [ -d $(DESTDIR)$(sysconfdir)/latch ]; then \
		echo "Removing latch conf directory..."; \
		rm -R $(DESTDIR)$(sysconfdir)/latch; \
	fi

