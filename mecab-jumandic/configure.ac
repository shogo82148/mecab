AC_INIT([mecab-jumandic],[7.0-20130310])
AC_CONFIG_SRCDIR([matrix.def])
AM_INIT_AUTOMAKE
AC_PROG_INSTALL
AC_SUBST(datarootdir)

AC_ARG_WITH(
    dicdir,
        [  --with-dicdir=DIR  set dicdir location ],
        AC_MSG_RESULT(using $with_dicdir for dicdir)
	MECAB_DICDIR=$with_dicdir, MECAB_DICDIR="no"
)

AC_ARG_WITH(
   mecab-config,
   [  --with-mecab-config=PATH  set mecab-config location [search path]],
   AC_MSG_RESULT(using $with_mecab_config for mecab-config)
   MECAB_CONFIG=$with_mecab_config,
   [AC_PATH_PROGS(MECAB_CONFIG, mecab-config, no)]
)

if test $MECAB_CONFIG = "no";
then
   AC_MSG_ERROR(mecab-config is not found in your system)
fi
AC_SUBST(MECAB_CONFIG)

if test $MECAB_DICDIR = "no";
then
   MECAB_DICDIR="`$MECAB_CONFIG --dicdir`/jumandic"
fi 

MECAB_DICT_INDEX="`$MECAB_CONFIG --libexecdir`/mecab-dict-index"
MECAB_MECABRC="`$MECAB_CONFIG --sysconfdir`/mecabrc"

AC_SUBST(MECAB_DICDIR)
AC_SUBST(MECAB_DICT_INDEX)
AC_SUBST(MECAB_MECABRC)

AC_ARG_WITH(
    charset,
     [  --with-charset=charset  set default charset (euc-jp/sjis/utf-8))],
      [CHARSET=${withval}], [CHARSET='utf-8']
)
CHARSET=$CHARSET
AC_SUBST(CHARSET)

MECAB_LEXICAL_DIC=`echo *.csv`
AC_SUBST(MECAB_LEXICAL_DIC)

MECAB_GENDATA="matrix.bin char.bin sys.dic unk.dic"
AC_SUBST(MECAB_GENDATA)

MECAB_PREDATA="`echo *.def` dicrc"
AC_SUBST(MECAB_PREDATA)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
