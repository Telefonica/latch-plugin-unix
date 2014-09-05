#!/bin/bash
#touch NEWS README AUTHORS ChangeLog COPYING
#autoreconf --install
#libtool
autoreconf --install
autoconf configure.ac > configure
