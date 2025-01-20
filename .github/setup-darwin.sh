#!/usr/bin/env bash

set -eux
brew install automake autoconf libtool gettext libiconv
echo ACLOCAL_PATH=/usr/local/opt/gettext/share/aclocal/ >> "$GITHUB_ENV"
echo LDFLAGS="-L/opt/homebrew/opt/libiconv/lib" >> "$GITHUB_ENV"
echo CPPFLAGS="-I/opt/homebrew/opt/libiconv/include" >> "$GITHUB_ENV"
