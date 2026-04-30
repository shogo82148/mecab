#!/bin/sh

echo "Running libtoolize ..."
libtoolize --force --copy \
  || glibtoolize --force --copy # fallback for macOS

echo "Searching for gettext/m4 ..."
gettext_m4_dir=""
for d in \
  /usr/share/gettext/m4 \
  /usr/local/share/gettext/m4 \
  /opt/homebrew/share/gettext/m4 \
  /usr/local/opt/gettext/share/gettext/m4 \
  /opt/homebrew/opt/gettext/share/gettext/m4
do
  if [ -d "$d" ]; then
    echo "Found gettext/m4 in $d"
    gettext_m4_dir="$d"
    break
  fi
done

echo "Running aclocal ..."
set -- -I .
[ -n "$gettext_m4_dir" ] && set -- "$@" -I "$gettext_m4_dir"
aclocal "$@"

echo "Running autoheader..."
autoheader

echo "Running automake ..."
automake --add-missing --copy

echo "Running autoconf ..."
set -- -i -f
[ -n "$gettext_m4_dir" ] && set -- "$@" -I "$gettext_m4_dir"
autoconf "$@"
