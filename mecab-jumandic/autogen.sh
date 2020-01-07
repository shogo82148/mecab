#!/bin/sh

echo "Running aclocal ..."
aclocal -I .
echo "Running automake ..."
automake --add-missing --copy
echo "Running autoconf ..."
autoconf -i -f
