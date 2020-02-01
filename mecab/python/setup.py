#!/usr/bin/env python

from distutils.core import setup,Extension
import subprocess

def cmd1(command):
    return subprocess.check_output(command, shell=True).decode().rstrip()

def cmd2(command):
    return cmd1(command).split()

setup(name = "mecab-python",
    version = cmd1("mecab-config --version"),
    py_modules=["MeCab"],
    ext_modules = [
        Extension("_MeCab",
            ["MeCab_wrap.cxx",],
            include_dirs=cmd2("mecab-config --inc-dir"),
            library_dirs=cmd2("mecab-config --libs-only-L"),
            libraries=cmd2("mecab-config --libs-only-l"))
        ])
