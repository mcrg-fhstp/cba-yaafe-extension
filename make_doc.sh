#!/bin/sh

sphinx-build -b text src_python/ .
mv FEATURES.txt FEATURES.md
