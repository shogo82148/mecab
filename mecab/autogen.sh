#!/bin/sh

echo "Running automake ..."
automake --add-missing --copy
echo "Running autoreconf ..."
autoreconf -i
