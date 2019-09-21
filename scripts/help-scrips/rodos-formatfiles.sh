#!/bin/bash

#astyle -O  -o  $*
clang-format   $* -i
\rm *.orig
