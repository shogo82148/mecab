#!/bin/sh

echo "Running libtoolize ..."
libtoolize --force --copy \
  || glibtoolize --force --copy # fallback for macOS
echo "Running aclocal ..."
aclocal -I . -I /usr/share/gettext/m4
echo "Running autoheader..."
autoheader
echo "Running automake ..."
automake --add-missing --copy
echo "Running autoconf ..."
autoconf -i -f -I /usr/share/gettext/m4
