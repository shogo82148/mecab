#!/usr/bin/env bash

set -eux
brew install automake autoconf libtool gettext libiconv
echo ACLOCAL_PATH="$(brew --prefix)/share/gettext/m4" >> "$GITHUB_ENV"
echo LDFLAGS="-L$(brew --prefix)/opt/libiconv/lib" >> "$GITHUB_ENV"
echo CPPFLAGS="-I$(brew --prefix)/opt/libiconv/include" >> "$GITHUB_ENV"
