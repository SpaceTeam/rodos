#!/bin/bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

# runs on Posix using 64 bit mode

export ARCH=posix64

SRCS[1]="${RODOS_SRC}/on-posix"
SRCS[2]="${RODOS_SRC}/on-posix/hal"

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/on-posix "  # only for platform-parameter.h 
export INCLUDES_TO_BUILD_LIB=" "

export LINKFLAGS=" -L ${RODOS_LIBS}/${ARCH} -lrodos -lm -lrt -lpthread "

#__________________________ Select one: gcc or clang, clang is better to detect warnings but slower
#export CPP_COMP="g++ "
#export C_COMP="gcc "  # only to compile BSP and Drivers from chip provider
export C_COMP="clang "  # only to compile BSP and Drivers from chip provider
export CPP_COMP="clang++ "  





