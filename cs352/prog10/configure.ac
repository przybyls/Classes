#
# Copyright 2011 Leo Przybylski
#
# Educational Community License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License. You may
# obtain a copy of the License at
#
# http://www.osedu.org/licenses/ECL-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an "AS IS"
# BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
# or implied. See the License for the specific language governing
# permissions and limitations under the License.
#

AC_INIT(prog10,prog10,0.0.1)
AC_CHECK_HEADERS([stdio.h])
AC_LANG_C
AC_PROG_CC
# AC_PROG_LIBTOOL
AM_INIT_AUTOMAKE

AC_ARG_WITH([debug-level],
  [AS_HELP_STRING([--with-debug-level],
    [level of debugging. Defaults to 3=ERROR, values are 0=DEBUG,1=INFO,2=WARN,3=ERROR,4=FATAL])],
  [DEBUG_CFLAGS="-DPROGRAM_LOG_LEVEL=$withval"])
AC_SUBST([DEBUG_CFLAGS])

AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h malloc.h strings.h unistd.h dlfcn.h)

AC_CHECK_LIB(ncurses, c_refresh, [], [
				])
#				exit -1
PACKAGE=prog10
VERSION=0.0.1


AC_OUTPUT(src/Makefile)
