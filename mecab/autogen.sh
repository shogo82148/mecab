#!/bin/sh

echo "Running aclocal ..."
aclocal -I .
echo "Running autoheader..."
autoheader
echo "Running automake ..."
automake --add-missing --copy
echo "Running autoreconf ..."
autoreconf -i -f
