#!/bin/sh

sphinx-build -b text -a src_python/ .
mv FEATURES.txt FEATURES.md
