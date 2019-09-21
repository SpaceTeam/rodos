#!/bin/bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

# runs on Linux, but has own scheduler and dispatcher
#      but it has no assembler for context switch, it uses makecontext

export ARCH=linux-makecontext

SRCS[1]="${RODOS_SRC}/bare-metal-generic"
SRCS[2]="${RODOS_SRC}/bare-metal/${ARCH}"
SRCS[3]="${RODOS_SRC}/bare-metal/${ARCH}/hal"

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/${ARCH} "  # only for platform-parameter.h
export INCLUDES_TO_BUILD_LIB=" -I ${RODOS_SRC}/bare-metal-generic "  

export CFLAGS=${CFLAGS}" -m32 "
export LINKFLAGS=" -L ${RODOS_LIBS}/${ARCH} -lrodos -lm "


#__________________________ Select one: gcc or clang, clang is better to detect warnings but slower
#export CPP_COMP="g++ "
#export C_COMP="gcc "  # only to compile BSP and Drivers from chip provider
export C_COMP="clang "  # only to compile BSP and Drivers from chip provider
export CPP_COMP="clang++ " 


