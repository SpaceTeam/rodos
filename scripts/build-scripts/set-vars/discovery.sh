#! /bin/bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

# used as name for the generated lib
export TARGET_LIB=discovery

# only for platform-parameter.h
export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/stm32f4/platform-parameter/discovery "

export OSC_CLK=8000000     # STM32F4Discovery, STM32F429Discovery
export SUB_ARCH=stm32f40x
source $RODOS_VARS_DIR/stm32f4.sh
