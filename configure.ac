#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([Qwertickle], [0.1.2], [aitjcize@gmail.com], [qwertickle], [http://github.com/Aitjcize/Qwertickle])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_SRCDIR([src/qwertickle.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AM_PATH_GLIB_2_0([2.16.0], [
	 GUI_CFLAGS="$GUI_CFLAGS $GLIB_CFLAGS"
	 GUI_LIBS="$GUI_LIBS $GLIB_LIBS"
	], [AC_MSG_ERROR([You need Glib >= 2.16.0 to build $PACKAGE])]) 

AM_PATH_GTK_2_0([2.16.0], [
	 GUI_CFLAGS="$GUI_CFLAGS $GTK_CFLAGS"
	 GUI_LIBS="$GUI_LIBS $GTK_LIBS"
	], [AC_MSG_ERROR([You need Gtk+ >= 2.16.0 to build $PACKAGE])]) 

PKG_CHECK_MODULES(GST, [gstreamer-0.10],,
	[AC_MSG_ERROR([You need libgstreamer0.10 to build $PACKAGE])]) 
GUI_CFLAGS="$GUI_CFLAGS $GST_CFLAGS"
GUI_LIBS="$GUI_LIBS $GST_LIBS"

PKG_CHECK_MODULES(XTST, [xtst],,
	[AC_MSG_ERROR([You need libXTst to build $PACKAGE])]) 
GUI_CFLAGS="$GUI_CFLAGS $XTST_CFLAGS"
GUI_LIBS="$GUI_LIBS $XTST_LIBS"

AC_SUBST(GUI_LIBS)
AC_SUBST(GUI_CFLAGS)

# Checks for header files.
AC_PATH_X

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CONFIG_FILES([Makefile
		 src/Makefile
		 data/Makefile
		 data/$PACKAGE.desktop
		 ])
AC_OUTPUT
