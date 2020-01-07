#!/usr/bin/env bash

set -eux
brew install automake autoconf libtool gettext
echo ::set-env name=ACLOCAL_PATH::/usr/local/opt/gettext/share/aclocal/
