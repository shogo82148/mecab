#!/usr/bin/env bash

set -eux
brew install automake autoconf libtool gettext libiconv
echo ACLOCAL_PATH=/opt/homebrew/share/gettext/m4 >> "$GITHUB_ENV"
echo LDFLAGS="-L/opt/homebrew/opt/libiconv/lib" >> "$GITHUB_ENV"
echo CPPFLAGS="-I/opt/homebrew/opt/libiconv/include" >> "$GITHUB_ENV"
