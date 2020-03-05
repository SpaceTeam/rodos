
#!/bin/bash
#_________________________________________Check it has to be sourced!
scriptName=$_
if [ $scriptName == $0 ]
then
   echo "_____________________________________________________ ERROR"
   echo "ERROR: Please call this script as source, and not as subshell"
   echo "like this:"
   echo "      %source $scriptName"
   echo "___________________________________________________________"
   exit
fi
#_____________________________________________________ Set Directories

export RODOS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export RODOS_ROOT=$RODOS_DIR  # sorry! Some times so, some times so!

export RODOS_BUILD_SCRIPTS=${RODOS_DIR}/scripts/build-scripts
export RODOS_HELP_SCRIPTS=${RODOS_DIR}/scripts/help-scrips

export RODOS_BUILD=${RODOS_DIR}/build       # place the build dir any where you want
export RODOS_TO_LINK=${RODOS_BUILD}/to_link
export RODOS_LIBS=${RODOS_BUILD}/libs

export RODOS_API=${RODOS_DIR}/api
export RODOS_SRC=${RODOS_DIR}/src
export RODOS_SUPPORT_LIBS=${RODOS_DIR}/support/support-libs
export RODOS_SUPPORT_PROGS=${RODOS_DIR}/support/support-programs
export RODOS_DOC=${RODOS_DIR}/doc

export PATH=${PATH}:${RODOS_BUILD_SCRIPTS}:${RODOS_HELP_SCRIPTS}:${RODOS_DIR}/support/tools:

alias cdrodos="cd ${RODOS_DIR}"

#_____________________________________________________ Set Default compiler flags

# you may extend or substitute this flags, but keep the names.

export INCLUDES=" -I ${RODOS_API} -I ${RODOS_SUPPORT_LIBS} ${APP_INCLUDES} "

#export CFLAGS=" -g -Wall -pedantic -Wno-long-long -Wno-cpp  -fno-exceptions -mfpmath=387 -O0 "
export CFLAGS=" -g -Wall -pedantic -Wno-long-long -Wno-cpp  -fno-exceptions -O0 "
export CPPFLAGS=" -fno-rtti -fno-exceptions  -std=c++14"

#_____________________________________________________ extras, nice to have but not required

cat << EOT2
______________________________________________________
   Now I know where RODOS is, you may use the rodos shell scripts
   from anywhere, and come back here using "cdrodos"
....
   if you are using git I recomment to use: 
   source ${RODOS_HELP_SCRIPTS}/set-git-aware-promt.sh
______________________________________________________
EOT2
