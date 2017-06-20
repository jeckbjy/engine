#!/bin/sh
find . -type f -exec enca -L zh_CN -x UTF-8 {} \;
