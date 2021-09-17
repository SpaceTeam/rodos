#! /bin/bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

# used as name for the generated lib
export TARGET_LIB=discovery_f411

# only for platform-parameter.h
export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/stm32f4/platform-parameter/STM32F411xE "

export OSC_CLK=8000000
export SUB_ARCH=stm32f411
source $RODOS_VARS_DIR/stm32f4.sh
