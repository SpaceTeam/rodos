#!/bin/bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

# the first parameter $1 is the architecture name, 
# all other parameter  ${@:2} are files to be compiled

#______________________________ test environment and parameter

source $(dirname $0)/../quit-if-environment-is-wrong.sh
source $(dirname $0)/../quit-if-arch-is-wrong.sh $1
 
source $VARS_FILE  # VARS_FILE i set in quit-if-arch-is-wrong.sh  as $1-set-vars.sh

#cppcheck      ${@:2} 
${CPP_COMP}   ${CFLAGS} ${CPPFLAGS} ${INCLUDES} -o tst ${@:2}  ${LINKFLAGS} 


