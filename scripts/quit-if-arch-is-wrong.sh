#!/bin/bash

VARS_FILE=$(dirname $0)/${1}-set-vars.sh

if [ ! -e $VARS_FILE ]; then
  cat << EOT

  *****************************************************
  *** ERROR
  ***   expected one parameter: name of the target achitecture 
  ***   Architectur $1 not defined
  ***   file $VARS_FILE not found
  ****************************************************

EOT
   exit
fi


