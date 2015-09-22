#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of RTL2832U Device.
#
# RTL2832U Device is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# RTL2832U Device is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#

AC_INIT(rh.RTL2832U, 2.0.0)
AM_INIT_AUTOMAKE([nostdinc foreign])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_CORBA_ORB
OSSIE_CHECK_OSSIE
OSSIE_SDRROOT_AS_PREFIX

AC_ARG_WITH(domainname,
	AS_HELP_STRING([--with-domainname=[domain name]], [Provide a domain name to the generated node, defaults to REDHAWK_DEV]),
	rtl2832u_node_domain=$withval,
	rtl2832u_node_domain=REDHAWK_DEV)
AC_SUBST(DOMAINNAME, $rtl2832u_node_domain)

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Dependencies
export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig"
PKG_CHECK_MODULES([PROJECTDEPS], [ossie >= 2.0 omniORB4 >= 4.1.0])
PKG_CHECK_MODULES([INTERFACEDEPS], [frontend >= 2.2, bulkio >= 2.0])
PKG_CHECK_MODULES([LIBRTLSDR], [librtlsdr = 0.5.2 libusb-1.0 >= 1.0.0 ])
PKG_CHECK_MODULES([LIBUUID], [uuid])
OSSIE_ENABLE_LOG4CXX
AX_BOOST_BASE([1.41])
AX_BOOST_SYSTEM
AX_BOOST_THREAD
AX_BOOST_REGEX

case $host_cpu in
  *arm*)
             CPU_SPECIFIC_FLAGS='-mfpu=neon -ftree-vectorize -ffast-math -funsafe-math-optimizations -funsafe-loop-optimizations -ftree-loop-if-convert-stores -mfloat-abi=hard -ftree-vectorizer-verbose=1'
             BINDIR_SUFFIX='_arm'
             ;;
  *x86*)
             CPU_SPECIFIC_FLAGS=''
             BINDIR_SUFFIX=''
             ;;
esac

AC_SUBST([CPU_SPECIFIC_FLAGS])
AC_SUBST([BINDIR_SUFFIX])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

