// MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
#ifndef MECAB_COMMON_H_
#define MECAB_COMMON_H_

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(_MSC_VER)
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
# if _MSC_VER < 1900
#  define snprintf _snprintf
# endif
#include <iterator>
#endif

#if defined(__CYGWIN__)
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#include <iterator>
#endif

#define COPYRIGHT "MeCab: Yet Another Part-of-Speech and Morphological Analyzer\n\
\nCopyright(C) 2001-2012 Taku Kudo \nCopyright(C) 2004-2008 Nippon Telegraph and Telephone Corporation\n"

#define SYS_DIC_FILE            "sys.dic"
#define UNK_DEF_FILE            "unk.def"
#define UNK_DIC_FILE            "unk.dic"
#define MATRIX_DEF_FILE         "matrix.def"
#define MATRIX_FILE             "matrix.bin"
#define CHAR_PROPERTY_DEF_FILE  "char.def"
#define CHAR_PROPERTY_FILE      "char.bin"
#define FEATURE_FILE            "feature.def"
#define REWRITE_FILE            "rewrite.def"
#define LEFT_ID_FILE            "left-id.def"
#define RIGHT_ID_FILE           "right-id.def"
#define POS_ID_FILE             "pos-id.def"
#define MODEL_DEF_FILE          "model.def"
#define MODEL_FILE              "model.bin"
#define DICRC                   "dicrc"
#define BOS_KEY                 "BOS/EOS"

#define DEFAULT_MAX_GROUPING_SIZE 24

#define CHAR_PROPERTY_DEF_DEFAULT "DEFAULT 1 0 0\nSPACE   0 1 0\n0x0020 SPACE\n"
#define UNK_DEF_DEFAULT           "DEFAULT,0,0,0,*\nSPACE,0,0,0,*\n"
#define MATRIX_DEF_DEFAULT        "1 1\n0 0 0\n"

#ifdef MECAB_USE_UTF8_ONLY
#define MECAB_DEFAULT_CHARSET "UTF-8"
#endif

#ifndef MECAB_DEFAULT_CHARSET
#if defined(_WIN32) && !defined(__CYGWIN__)
#define MECAB_DEFAULT_CHARSET "SHIFT-JIS"
#else
#define MECAB_DEFAULT_CHARSET "EUC-JP"
#endif
#endif

#define NBEST_MAX 512
#define NODE_FREELIST_SIZE 512
#define PATH_FREELIST_SIZE 2048
#define MIN_INPUT_BUFFER_SIZE 8192
#define MAX_INPUT_BUFFER_SIZE (8192*640)
#define BUF_SIZE 8192

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifdef UNICODE
#define WPATH(path) (MeCab::Utf8ToWide(path).c_str())
#else
#define WPATH(path) (path)
#endif

#ifndef MECAB_FALLTHROUGH
#  if defined(HAVE_CXX17_FALLTHROUGH_ATTRIBUTE)
#    define MECAB_FALLTHROUGH [[fallthrough]]
#  elif defined(HAVE_GNU_FALLTHROUGH_ATTRIBUTE)
#    define MECAB_FALLTHROUGH [[gnu::fallthrough]]
#  elif defined(HAVE_CLANG_FALLTHROUGH_ATTRIBUTE)
#    define MECAB_FALLTHROUGH [[clang::fallthrough]]
#  elif defined(HAVE_GCC_FALLTHROUGH_ATTRIBUTE)
#    define MECAB_FALLTHROUGH __attribute__((fallthrough))
#  else
#    define MECAB_FALLTHROUGH ((void)0)
#  endif
#endif

namespace MeCab {
class die {
 public:
  die() {}
  ~die() {
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
  }
  int operator&(std::ostream&) { return 0; }
};

struct whatlog {
  std::ostringstream stream_;
  std::string str_;
  const char *str() {
    str_ = stream_.str();
    return str_.c_str();
  }
};

class wlog {
 public:
  wlog(whatlog *what) : what_(what) {
    what_->stream_.clear();
  }
  bool operator&(std::ostream &) {
    return false;
  }
 private:
  whatlog *what_;
};
}  // MeCab

#define WHAT what_.stream_

#define CHECK_FALSE(condition) \
 if (condition) {} else return \
   wlog(&what_) & what_.stream_ <<              \
      __FILE__ << "(" << __LINE__ << ") [" << #condition << "] "

#define CHECK_DIE(condition) \
(condition) ? 0 : die() & std::cerr << __FILE__ << \
"(" << __LINE__ << ") [" << #condition << "] "

// die when overflow. the value shoule be size_t.
#define CAST_OR_DIE(target, value) \
  (value)<=static_cast<size_t>(std::numeric_limits<target>::max()) ? \
  static_cast<target>(value) : \
  static_cast<target>(die() & std::cerr << __FILE__ << \
  "(" << __LINE__ << ") [" << #value << "] overflow")

#endif  // MECAB_COMMON_H_
