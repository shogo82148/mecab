#!/bin/sh

echo "Running libtoolize ..."
libtoolize --force --copy
echo "Running aclocal ..."
aclocal -I .
echo "Running autoheader..."
autoheader
echo "Running automake ..."
automake --add-missing --copy
echo "Running autoconf ..."
autoconf -i -f
