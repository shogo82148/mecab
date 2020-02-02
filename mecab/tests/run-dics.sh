#!/bin/sh

DIR="shiin t9 latin katakana autolink chartype ngram"

for dir in $DIR
do
  (cd "$dir" || exit 1
  ../../src/mecab-dict-index -f euc-jp -c euc-jp;
  ../../src/mecab -r /dev/null -d . test > test.out;
  if diff -b test.gld test.out;
  then
    : success
  else
    echo "runtests faild in $dir"
    exit 1
  fi;
  rm -f -- *.bin *.dic test.out)
done

exit 0
