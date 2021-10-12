#!/usr/bin/env bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019


if [ -z $TARGET_LIB ]; then
  cat << EOT1
  ****************
  *** Do not use this architecture directly, please use
  *** one of its derivates:
  ***    skith
  ***    discovery
  ***    discovery_f411
  ***    discovery_f429
  *********************
EOT1
exit 
fi



export ARCH=stm32f4         #used to select compile directories

# From here, we can have different sub achictures. We use the
# STM32F407VGT6 [168MHz, 192KByte SRAM == Discovery board] as default

if [ -z $SUB_ARCH ] ; then
    # did not get SUB_ARCH override => using default
    SUB_ARCH=stm32f40x

    # possible values are:
    # -> stm32f429   (STM32F429Discovery)
    # -> stm32f427   (STM32F427ZI)
    # -> stm32f411   (STM32F411CE)
    # -> stm32f40x   (STM32F4Discovery, MINI-M4)
    # -> stm32f401   (STM32F401 NUCLEO)
fi

if [ -z $OSC_CLK ] ; then
    # did not get OSC_CLK override => using default
    OSC_CLK=8000000

    # possible values are:
    # -> 8000000     (STM32F4Discovery, STM32F429Discovery, STM32F411Discovery)
    # -> 16000000    (MINI-M4, SKITH)
    # -> 24000000    (PX4FLOW, HoneyCloud)
fi

if [ -z $LINKER_SCRIPT ] ; then
    # did not get LINKER_SCRIPT override => using default
    if [ ${SUB_ARCH} = stm32f429 ]; then
        LINKER_SCRIPT="${RODOS_SRC}/bare-metal/${ARCH}/scripts/stm32f429_flash.ld"
    elif [ ${SUB_ARCH} = stm32f427 ]; then
        LINKER_SCRIPT="${RODOS_SRC}/bare-metal/${ARCH}/scripts/stm32f427zi_flash.ld"
    elif [ ${SUB_ARCH} = stm32f411 ]; then
        LINKER_SCRIPT="${RODOS_SRC}/bare-metal/${ARCH}/scripts/stm32f411xe_flash.ld"
    elif [ ${SUB_ARCH} = stm32f401 ]; then
        LINKER_SCRIPT="${RODOS_SRC}/bare-metal/${ARCH}/scripts/stm32f401_flash.ld"
    else
        LINKER_SCRIPT="${RODOS_SRC}/bare-metal/${ARCH}/scripts/stm32_flash.ld"
    fi
fi

if [ -z $ST_LINK ] ; then
    if [ ${SUB_ARCH} = stm32f401 ]; then
        export ST_LINK="v2_0"
    else
        export ST_LINK="v2_1"
    fi
fi

SRCS[1]="${RODOS_SRC}/bare-metal-generic"
SRCS[2]="${RODOS_SRC}/bare-metal/${ARCH}"
SRCS[3]="${RODOS_SRC}/bare-metal/${ARCH}/hal"
SRCS[4]="${RODOS_SRC}/bare-metal/${ARCH}/startup"
if [ ${SUB_ARCH} = stm32f411 ]; then
  SRCS[5]="${RODOS_SRC}/bare-metal/${ARCH}/startup/stm32f411xe"
else
  SRCS[5]="${RODOS_SRC}/bare-metal/${ARCH}/startup/stm32f4xx"
fi
SRCS[6]="${RODOS_SRC}/bare-metal/${ARCH}/STM32F4xx_StdPeriph_Driver/src"

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/${ARCH} "

export INCLUDES_TO_BUILD_LIB=" \
    -I ${RODOS_SRC}/bare-metal-generic \
    -I ${RODOS_SRC}/bare-metal/${ARCH} \
    -I ${RODOS_SRC}/bare-metal/${ARCH}/CMSIS/Device/ST/STM32F4xx/Include \
    -I ${RODOS_SRC}/bare-metal/${ARCH}/CMSIS/Include \
    -I ${RODOS_SRC}/bare-metal/${ARCH}/STM32F4xx_StdPeriph_Driver/inc "


COMMON_CFLAGS=" -g3 -gdwarf-2 -DHSE_VALUE=${OSC_CLK} -DUSE_STDPERIPH_DRIVER"
COMMON_LINKFLAGS=" -nostartfiles -nodefaultlibs -nostdlib \
        -Xlinker --gc-sections \
        -fno-unwind-tables -fno-asynchronous-unwind-tables \
        -L${RODOS_LIBS}/${TARGET_LIB} \
        -lrodos -lc -lm -lgcc"
COMMON_HWCFLAGS=" -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16"

export CPPFLAGS=${CPPFLAGS}" -std=gnu++14"
export LINKFLAGS=${COMMON_LINKFLAGS}" -Wl,-T${LINKER_SCRIPT}"

if [ ${SUB_ARCH} = stm32f40x ]; then
    export HWCFLAGS="${COMMON_HWCFLAGS} -mfloat-abi=softfp"
    export CFLAGS=${CFLAGS}"${COMMON_CFLAGS} ${HWCFLAGS} \
        -DSTM32F40_41xxx \
        -DUSE_STM32_DISCOVERY"
elif [ ${SUB_ARCH} = stm32f429 ]; then
    export HWCFLAGS="${COMMON_HWCFLAGS} -mfloat-abi=softfp"
    export CFLAGS=${CFLAGS}" ${COMMON_CFLAGS} ${HWCFLAGS} \
        -DSTM32F429_439xx \
        -DUSE_STM32F429I_DISCO"
elif [ ${SUB_ARCH} = stm32f427 ]; then
    export HWCFLAGS="${COMMON_HWCFLAGS} -mfloat-abi=softfp"
    export CFLAGS=${CFLAGS}" ${COMMON_CFLAGS} ${HWCFLAGS} \
        -DSTM32F427_437xx"
elif [ ${SUB_ARCH} = stm32f411 ]; then
    export HWCFLAGS="${COMMON_HWCFLAGS} -mfloat-abi=softfp"
    export CFLAGS=${CFLAGS}" ${COMMON_CFLAGS} ${HWCFLAGS} \
        -DSTM32F411xE \
        -DUSE_STM32_DISCOVERY \
        -DHSE_STARTUP_TIMEOUT=10000000"
elif [ ${SUB_ARCH} = stm32f401 ]; then
    export HWCFLAGS="${COMMON_HWCFLAGS} -mfloat-abi=softfp"
    export CFLAGS=${CFLAGS}" ${COMMON_CFLAGS} ${HWCFLAGS} \
        -DSTM32F401xx"
fi

#export ARM_TOOLS="/opt/arm-tools/bin/"
export ARM_TOOLS=""

export CPP_COMP="${CXX:-${ARM_TOOLS}arm-none-eabi-g++} "
export C_COMP="${CC:-${ARM_TOOLS}arm-none-eabi-gcc} "
export AR="${AR:-${ARM_TOOLS}arm-none-eabi-ar} "
export OBJCOPY="${OBJCOPY:-${ARM_TOOLS}arm-none-eabi-objcopy} "
export OBJDUMP="${OBJDUMP:-${ARM_TOOLS}arm-none-eabi-objdump} "
