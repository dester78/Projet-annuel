
AC_PREREQ([2.69])
AC_INIT([CScan],[1.0],[support@fight-food-waste.ovh])

AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.10 foreign dist-bzip2 no-dist-gzip])

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])


# Check for programs    
AX_CHECK_ENABLE_DEBUG
AC_PROG_CC
AC_HEADER_STDC

# Initialize libtool
LT_PREREQ([2.2.6])
LT_INIT


PKG_CHECK_MODULES([CSCAN], gtk+-3.0  libcurl bluez )


GETTEXT_PACKAGE=CScan

IT_PROG_INTLTOOL([0.40.0])

AC_SUBST([GETTEXT_PACKAGE], [CScan])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], "$GETTEXT_PACKAGE", [Define to the gettext package name.])




# CSCAN_LOCALEDIR=[${datadir}/locale]
# AC_SUBST(CSCAN_LOCALEDIR)
# *************************************
# *************************************

AC_CONFIG_FILES([
Makefile
src/Makefile
ca/Makefile
css/Makefile
logs/Makefile
glade/Makefile
json/Makefile
po/Makefile.in
])

AC_OUTPUT
