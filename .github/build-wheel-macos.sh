#!/usr/bin/env bash
# build wheels for macOS
# based on https://github.com/SamuraiT/mecab-python3/blob/5575170dc71dc5d4e0c79a23c20d71edad5c2845/.github/macos-build.sh

set -eou pipefail

FLAGS="--enable-utf8-only"
X86_TRIPLET=x86_64-apple-macos10.9
ARM_TRIPLET=arm64-apple-macos11


tar xzvf "dist/mecab/mecab-$MECAB_VERSION.tar.gz"

# build MeCab
(
  cd "mecab-$MECAB_VERSION"
  rm -rf src/.libs-arm64 src/.libs-x86_64 src/.libs.combined

  ./configure $FLAGS --host="arm-apple-darwin24.3.0 " CXX="clang++ -target $ARM_TRIPLET" CC="clang"

  make clean
  # nproc doesn't exist on the runner
  make -j"$(sysctl -n hw.logicalcpu_max)"

  mv src/.libs src/.libs-arm64

  ./configure $FLAGS --host="x86_64-apple-darwin24.3.0 " CXX="clang++ -target $X86_TRIPLET" CC="clang"

  make clean
  make -j"$(sysctl -n hw.logicalcpu_max)"

  mv src/.libs src/.libs-x86_64

  rm -rf src/.libs.combined
  mkdir src/.libs.combined

  # lipo is an osx utility to create universal binaries
  lipo -create src/.libs-arm64/libmecab.2.dylib src/.libs-x86_64/libmecab.2.dylib -output src/.libs.combined/libmecab.2.dylib

  lipo -create src/.libs-arm64/libmecab.a src/.libs-x86_64/libmecab.a -output src/.libs.combined/libmecab.a

  cp src/.libs-arm64/libmecab.lai src/.libs.combined/libmecab.lai

  ls src/.libs-arm64/*.o src/.libs-arm64/mecab* | while read line; do
      echo $line
      lipo -create $line "src/.libs-x86_64/$(basename $line)" -output "src/.libs.combined/$(basename $line)"
  done

  cd src/.libs.combined
  ln -s ../libmecab.la libmecab.la
  ln -s libmecab.2.dylib libmecab.dylib
  cd ../..
  mv src/.libs.combined src/.libs

  sudo make install
)

pwd
ls -la

# install Python binding
tar xzvf "dist/scripts/mecab-python-$MECAB_VERSION.tar.gz"
(
  cd "mecab-python-$MECAB_VERSION"
  python -m pip install --upgrade setuptools wheel pip setuptools-scm
  python -m pip install cibuildwheel==2.23.0

  export CIBW_BUILD="cp38-* cp39-* cp310-* cp311-* cp312-* cp313-*"
  python -m cibuildwheel --platform macos --archs x86_64,arm64,universal2 --output-dir ../dist
)
