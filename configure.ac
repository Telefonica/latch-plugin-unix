#
# Configure.ac script for automake.
# Copyright (C) 2013 Eleven Paths

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#


AC_PREREQ([2.61])
AC_INIT([latch], [1.1], [latch-help@support.elevenpaths.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE([subdir-objects])
AM_MAINTAINER_MODE


LT_PREREQ([2.2])
LT_INIT 
AC_ENABLE_SHARED
AC_DISABLE_STATIC

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

AC_CHECK_HEADERS([curl/curl.h], [],
  [AC_MSG_ERROR([[libcurl4 header files not found, install libcurl4-openssl-dev.]])])

AC_CHECK_HEADERS([openssl/hmac.h openssl/bio.h openssl/evp.h openssl/buffer.h], [],
  [AC_MSG_ERROR([[openssl header files not found, install openssl-dev.]])])

AC_CHECK_HEADERS([security/pam_appl.h], [],
  [AC_MSG_ERROR([[PAM header files not found, install libpam-dev.]])])

AC_CHECK_HEADERS([security/pam_modules.h security/_pam_macros.h security/pam_modutil.h security/openpam.h security/pam_misc.h], [], [],
  [#include <sys/types.h>
   #include <security/pam_appl.h>])

# Checks for libraries.
AC_CHECK_LIB([crypto], [HMAC])
AC_CHECK_LIB([curl], [curl_easy_setopt])
AC_CHECK_LIB([pam], [pam_start])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strchr strerror strstr])

# Define latch conf dirrectory
latchsysconfdir=$sysconfdir/latch
AC_DEFINE_DIR([LATCH_CONF_DIR], [latchsysconfdir], [Latch configuration directory])

AC_SUBST(PAMDIR, "\$(exec_prefix)/lib/security")
AC_ARG_WITH(pam-dir,
  AC_HELP_STRING([--with-pam-dir=DIR],
                 [Where to install PAM module [[PREFIX/lib/security]]]),
            [case "${withval}" in
            /*) PAMDIR="${withval}";;
            ./*|../*) AC_MSG_ERROR(Bad value for --with-pam-dir);;
            *)  PAMDIR="\$(exec_prefix)/lib/${withval}";;
            esac])

AC_ARG_ENABLE([gcc-warnings],
  [AS_HELP_STRING([--enable-gcc-warnings],
      [turn on lots of GCC warnings (for developers)])],
  [case $enableval in
     yes|no) ;;
     *)      AC_MSG_ERROR([bad value $enableval for gcc-warnings option]) ;;
   esac
   gl_gcc_warnings=$enableval],
  [gl_gcc_warnings=no]
)

#############################
#
# Platform specific setup
#
#############################

AH_TEMPLATE([HPUX], [HP-UX System])
AH_TEMPLATE([DARWIN], [Darwin System])
AH_TEMPLATE([OPENBSD], [OpenBSD System])
AH_TEMPLATE([NETBSD], [NetBSD System])
AH_TEMPLATE([FREEBSD], [FreeBSD System])
AH_TEMPLATE([SOLARIS_251], [Solaris 2.51 System])
AH_TEMPLATE([SOLARIS_26], [Solaris 2.6 System])
AH_TEMPLATE([SOLARIS_27], [Solaris 2.7 System])
AH_TEMPLATE([SOLARIS_28], [Solaris 2.8 System])
AH_TEMPLATE([SOLARIS_29], [Solaris 2.9 System])
AH_TEMPLATE([SOLARIS], [Solaris System])
AH_TEMPLATE([LINUX], [Linux System])
AH_TEMPLATE([LINUX_20], [Linux 2.0 System])
AH_TEMPLATE([LINUX_21], [Linux 2.1 System])
AH_TEMPLATE([LINUX_22], [Linux 2.2 System])
AH_TEMPLATE([LINUX_23], [Linux 2.3 System])
AH_TEMPLATE([LINUX_24], [Linux 2.4 System])
AH_TEMPLATE([LINUX_25], [Linux 2.5 System])
AH_TEMPLATE([LINUX_26], [Linux 2.6 System])
AH_TEMPLATE([LINUX_30], [Linux 3.0 System])
AH_TEMPLATE([LINUX_31], [Linux 3.1 System])
AH_TEMPLATE([LINUX_32], [Linux 3.2 System])
AH_TEMPLATE([DARWIN_106], [OSX 10.6])
AH_TEMPLATE([DARWIN_107], [OSX 10.7])
AH_TEMPLATE([DARWIN_108], [OSX 10.8])
AH_TEMPLATE([DARWIN_109], [OSX 10.9])

case "$host_os" in

   *linux*)
           AC_DEFINE(LINUX)
           linux=true
           case "`uname -r`" in
                3.2*)
                   AC_DEFINE(LINUX_32)
                ;;
                3.1*)
                   AC_DEFINE(LINUX_31)
                ;;
                3.0*)
                   AC_DEFINE(LINUX_30)
                ;;
                2.6*)
                   AC_DEFINE(LINUX_26)
                ;;
                2.5*)
                   AC_DEFINE(LINUX_25)
                ;;
                2.4*)
                   AC_DEFINE(LINUX_24)
                ;;
                2.3*)
                   AC_DEFINE(LINUX_23)
                ;;
                2.2*)
                   AC_DEFINE(LINUX_22)
                ;;
                2.1*)
                   AC_DEFINE(LINUX_21)
                ;;
                2.0*)
                   AC_DEFINE(LINUX_20)
                ;;
           esac
           ;;

   *solaris*)
           AC_DEFINE(SOLARIS)
           case "`uname -r`" in
                5.5.1)
                   AC_DEFINE(SOLARIS_251)
                ;;
                5.6*)
                   AC_DEFINE(SOLARIS_26)
                ;;
                5.7*)
                   AC_DEFINE(SOLARIS_27)
                ;;
                5.8*)
                   AC_DEFINE(SOLARIS_27)
                ;;
                5.9*)
                   AC_DEFINE(SOLARIS_27)
                ;;
           esac
           ;;

   *freebsd*)
           AC_DEFINE(FREEBSD)
           ;;

   *netbsd*)
           AC_DEFINE(NETBSD)
           ;;

   *openbsd*)
           AC_DEFINE(OPENBSD)
           ;;

   *hpux*)
           AC_DEFINE(HPUX)
           ;;
   *darwin*)
           AC_DEFINE(DARWIN)
           darwin=true
           case "`sw_vers -productVersion`" in
               10.6*)
                   AC_DEFINE(DARWIN_106)
               ;;
               10.7*)
                   AC_DEFINE(DARWIN_107)
               ;;
               10.8*)
                   AC_DEFINE(DARWIN_108)
               ;;
               10.9*)
                   AC_DEFINE(DARWIN_109)
               ;;
           esac
           ;;

   *)
       AC_MSG_WARN(it seems that your OS is not supported)
    ;;

esac
AM_CONDITIONAL([OSLINUX], [test x$linux = xtrue])
AM_CONDITIONAL([OSDARWIN], [test x$darwin = xtrue])

AC_MSG_NOTICE([Summary of build options:

  Version:            ${VERSION}
  Host type:          ${host}
  Install prefix:     ${prefix}
  Compiler:           ${CC}
])


AC_CONFIG_FILES([
Makefile
src/Makefile
pam/Makefile
modules/SSH/src/Makefile
])

AC_OUTPUT

