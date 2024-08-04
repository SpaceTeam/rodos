#!/usr/bin/env bash

cd /workspaces/STS1_COBC_SW/rodos
rm -r ./build/cobc
cmake --toolchain cmake/port/cobc.cmake -S . -B build/cobc
cmake --build build/cobc
sudo cmake --install build/cobc --prefix "/usr/local/stm32f411"
