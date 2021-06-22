#!/bin/bash

# https://developer.moe/linux-cross-compile-toolchain-on-cygwin#files
export TOOLCHAIN_NAME="i686-unknown-linux-gnu"
export TOOLCHAIN_VERSION="10.2.0"

# Extract the tar archive
tar xzfv ${TOOLCHAIN_NAME}-${TOOLCHAIN_VERSION}.tar.gz

# Populate directory
chmod -R u+w /usr/local
./${TOOLCHAIN_NAME}/bin/${TOOLCHAIN_NAME}-populate -m -s ${TOOLCHAIN_NAME} -d /usr/local/${TOOLCHAIN_NAME} &>/dev/null

# Remove temporary directory & .config file
rm -r -f ./${TOOLCHAIN_NAME} | true
rm -f /usr/local/.config | true

# Remove tool chain archive
# rm -r -f ${TOOLCHAIN_NAME}-${TOOLCHAIN_VERSION}.tar.gz | true


ln -s /usr/local/i686-unknown-linux-gnu/bin/i686-unknown-linux-gnu-ar.exe /bin/i686-unknown-linux-gnu-ar.exe
ln -s /usr/local/i686-unknown-linux-gnu/bin/i686-unknown-linux-gnu-as.exe /bin/i686-unknown-linux-gnu-as.exe
ln -s /usr/local/i686-unknown-linux-gnu/bin/i686-unknown-linux-gnu-gcc.exe /bin/i686-unknown-linux-gnu-gcc.exe
ln -s /usr/local/i686-unknown-linux-gnu/bin/i686-unknown-linux-gnu-g++.exe /bin/i686-unknown-linux-gnu-g++.exe

echo
#echo "Links for toolchain were successfully created!"
