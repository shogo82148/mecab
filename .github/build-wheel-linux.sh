#!/usr/bin/env bash
# build wheels for manylinux/musllinux
# based on https://github.com/SamuraiT/mecab-python3/blob/5575170dc71dc5d4e0c79a23c20d71edad5c2845/.github/workflows/entrypoint.sh

set -euxo pipefail

# unarchive MeCab
MECAB_VERSION=$(</mecab/dist/mecab-version.txt)
tar xzvf "/mecab/dist/mecab/mecab-${MECAB_VERSION}.tar.gz"

# Install MeCab
cd "mecab-${MECAB_VERSION}"
./configure --enable-utf8-only
make -j"$(nproc)"
make install

# unarchive Python binding
cd /
tar xzvf "/mecab/dist/scripts/mecab-python-${MECAB_VERSION}.tar.gz"

# Build the wheels
for PYVER in cp38-cp38 cp39-cp39 cp310-cp310 cp311-cp311 cp312-cp312 cp313-cp313; do
  "/opt/python/${PYVER}/bin/pip" wheel "/mecab-python-${MECAB_VERSION}" -w /mecab/wheels
done

# fix the wheels (bundles libs)
for wheel in /mecab/wheels/*.whl; do
  auditwheel repair "$wheel" -w /mecab/linux-wheels
done
