#!/usr/bin/env bash

set -eux
brew install automake autoconf libtool gettext libiconv
echo ACLOCAL_PATH=/usr/local/opt/gettext/share/aclocal/ >> "$GITHUB_ENV"
