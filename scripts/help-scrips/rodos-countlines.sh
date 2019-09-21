#!/bin/bash

cd $RODOS_DIR
decomment api/*.h src/bare-metal-generic/*.h  src/independent/*.cpp src/independent/gateway/*.cpp \
          src/bare-metal-generic/*.cpp src/bare-metal/linux-x86/*.h src/bare-metal/linux-x86/*.cpp   | wc 

# 28.8.2019: 4166

