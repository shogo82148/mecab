#!/bin/sh

#
dir="eval"
cd "$dir" || exit 1
../../src/mecab-system-eval --level="0 1 2 3 4" system answer > test.out
if --strip-trailing-cr diff test.gld test.out;
then
  : success!
else
  echo "runtests faild in $dir"
  exit 1
fi;
rm -f test.out
exit 0
