dnl $Id$
dnl config.m4 for extension afk

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(afk, for afk support,
dnl Make sure that the comment is aligned:
dnl [  --with-afk             Include afk support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(afk, whether to enable afk support,
dnl Make sure that the comment is aligned:
dnl [  --enable-afk           Enable afk support])

if test "$PHP_AFK" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-afk -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/afk.h"  # you most likely want to change this
  dnl if test -r $PHP_AFK/$SEARCH_FOR; then # path given as parameter
  dnl   AFK_DIR=$PHP_AFK
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for afk files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       AFK_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$AFK_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the afk distribution])
  dnl fi

  dnl # --with-afk -> add include path
  dnl PHP_ADD_INCLUDE($AFK_DIR/include)

  dnl # --with-afk -> check for lib and symbol presence
  dnl LIBNAME=afk # you may want to change this
  dnl LIBSYMBOL=afk # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $AFK_DIR/lib, AFK_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_AFKLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong afk lib version or lib not found])
  dnl ],[
  dnl   -L$AFK_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(AFK_SHARED_LIBADD)

  PHP_NEW_EXTENSION(afk, afk.c, $ext_shared)
fi
