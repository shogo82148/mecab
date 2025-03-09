#!/usr/bin/env python

from setuptools import setup, Extension
import subprocess
import os

def cmd1(command):
    return subprocess.check_output(command, shell=True).decode().rstrip()

def cmd2(command):
    return cmd1(command).split()

# Windows requires special prep
if os.name == 'nt':
    data_files = [("lib/site-packages",
                  ["libmecab.dll"])]
else:
    data_files = []

setup(
    version = cmd1("mecab-config --version"),
    py_modules=["MeCab"],
    ext_modules = [
        Extension("_MeCab",
            ["MeCab_wrap.cxx",],
            include_dirs=cmd2("mecab-config --inc-dir"),
            library_dirs=cmd2("mecab-config --libs-only-L"),
            libraries=cmd2("mecab-config --libs-only-l"))
        ],
    data_files=data_files)
