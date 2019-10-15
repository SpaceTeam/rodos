#!/bin/bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

# runs on Posix using 32 bit mode

export ARCH=posix

SRCS[1]="${RODOS_SRC}/on-posix"
SRCS[2]="${RODOS_SRC}/on-posix/hal"

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/on-posix "  # only for platform-parameter.h 
export INCLUDES_TO_BUILD_LIB=" "

export CFLAGS=${CFLAGS}" -m32 "
export LINKFLAGS=" -L ${RODOS_LIBS}/${ARCH} -lrodos -lm -lrt -lpthread "


#__________________________ Select one: gcc or clang, clang is better to detect warnings but slower
#export CPP_COMP="g++ "
#export C_COMP="gcc "  # only to compile BSP and Drivers from chip provider
export C_COMP="${CC:-clang} "  # only to compile BSP and Drivers from chip provider
export CPP_COMP="${CXX:-clang++} "  



#POSIX-warning: 
#To execute your compiled code, please use no more than one core. 
#Please call
#    "sudo taskset -c 0 tst"


