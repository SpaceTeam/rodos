#!/usr/bin/env bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

#used as name for the generated lib
export TARGET_LIB=skith

# only for platform-parameter.h
export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/stm32f4/platform-parameter/skith "

export OSC_CLK=16000000     # SKITH
export SUB_ARCH=stm32f40x
source $RODOS_VARS_DIR/stm32f4.sh
