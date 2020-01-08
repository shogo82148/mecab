#!/usr/bin/env bash

set -eux

ROOT=$(cd "$(dirname "$0")" && cd .. && pwd)

cd "$ROOT/mecab"

# build mecab
./autogen.sh
./configure --enable-utf8-only
make

rm -rf "$ROOT/mecab/doc"/*.html "$ROOT/mecab/doc"/*.png "$ROOT/mecab/doc"/doxygen
hugo

cd "$ROOT/mecab/doc"
doxygen mecab.cfg
make update-html

cd "$ROOT/mecab/man"
make update-man

cd "$ROOT/mecab"
make distclean

rm -rf "$ROOT/docs"
cp -r "$ROOT/mecab/doc" "$ROOT/docs"
