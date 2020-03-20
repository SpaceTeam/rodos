#! /bin/bash

# Author Marcelo Alves (Uni Minho), Oct 2019
# 	 Jorge Oliveira (Uni Minho)

export ARCH=efr32fg1p    #used to select compile directories
export TARGET_LIB=gecko  #used as name for the generated lib

#---------------------------------------------- from here the same for all sub architectures

RODOS_ARCH_SRC1="${RODOS_SRC}/bare-metal/${ARCH}"
RODOS_ARCH_SRC2="${RODOS_SRC}/bare-metal/${ARCH}/emlib/inc"
RODOS_ARCH_SRC3="${RODOS_SRC}/bare-metal/${ARCH}/CMSIS/Include"
RODOS_ARCH_SRC4="${RODOS_SRC}/bare-metal/${ARCH}/EFR32FG1P/Include"
RODOS_ARCH_SRC5="${RODOS_SRC}/bare-metal/${ARCH}/hal"
RODOS_ARCH_SRC6="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/common/inc"
RODOS_ARCH_SRC7="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/dmadrv/inc"
RODOS_ARCH_SRC8="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/sleep/inc"
RODOS_ARCH_SRC9="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/ustimer/inc"
RODOS_ARCH_SRC10="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/ustimer/config"
RODOS_ARCH_SRC11="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/tempdrv/inc"
RODOS_ARCH_SRC12="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/tempdrv/config"
RODOS_ARCH_SRC13="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/dmadrv/config"
RODOS_ARCH_SRC14="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/gpiointerrupt/inc"
RODOS_ARCH_SRC15="${RODOS_SRC}/bare-metal/${ARCH}/rail_lib/common"
RODOS_ARCH_SRC16="${RODOS_SRC}/bare-metal/${ARCH}/rail_lib/hal"
RODOS_ARCH_SRC17="${RODOS_SRC}/bare-metal/${ARCH}/rail_lib/hal/efr32"
RODOS_ARCH_SRC18="${RODOS_SRC}/bare-metal/${ARCH}/rail_lib/chip/efr32/efr32xg1x"
RODOS_ARCH_SRC19="${RODOS_SRC}/bare-metal/${ARCH}/hal-config"
RODOS_ARCH_SRC20="${RODOS_SRC}/bare-metal/${ARCH}/sleeptimer/inc"
RODOS_ARCH_SRC21="${RODOS_SRC}/bare-metal/${ARCH}/sleeptimer/config"
RODOS_ARCH_SRC22="${RODOS_SRC}/bare-metal/${ARCH}/common/inc"
RODOS_ARCH_SRC23="${RODOS_SRC}/bare-metal/${ARCH}/config"
RODOS_ARCH_SRC24="${RODOS_SRC}/bare-metal/${ARCH}/rail-config"

SRCS[1]="${RODOS_SRC}/bare-metal-generic"
SRCS[2]="${RODOS_SRC}/bare-metal/${ARCH}"
SRCS[3]="${RODOS_SRC}/bare-metal/${ARCH}/emlib/src"
SRCS[4]="${RODOS_ARCH_SRC1}"
SRCS[5]="${RODOS_SRC}/bare-metal/${ARCH}/EFR32FG1P/Source/GCC"
SRCS[6]="${RODOS_SRC}/bare-metal/${ARCH}/EFR32FG1P/Source"
SRCS[7]="${RODOS_SRC}/bare-metal/${ARCH}/hal"
SRCS[8]="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/dmadrv/src"
SRCS[9]="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/gpiointerrupt/src"
SRCS[10]="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/sleep/src"
SRCS[11]="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/tempdrv/src"
SRCS[12]="${RODOS_SRC}/bare-metal/${ARCH}/emdrv/ustimer/src"
SRCS[13]="${RODOS_SRC}/bare-metal/${ARCH}/sleeptimer/src"
SRCS[14]="${RODOS_SRC}/bare-metal/${ARCH}/rail_lib/hal"
SRCS[15]="${RODOS_SRC}/bare-metal/${ARCH}/rail_lib/hal/efr32"
SRCS[16]="${RODOS_SRC}/bare-metal/${ARCH}/common/src"
SRCS[17]="${RODOS_SRC}/bare-metal/${ARCH}/common/bsp"
SRCS[18]="${RODOS_SRC}/bare-metal/${ARCH}/common/drivers/used"
SRCS[19]="${RODOS_SRC}/bare-metal/${ARCH}/rail-config"

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/${ARCH} \
	-I${RODOS_SRC}/bare-metal/${ARCH}/EFR32FG1P/Include \
	-I${RODOS_SRC}/bare-metal/${ARCH}/CMSIS/Include \
	-I${RODOS_SRC}/bare-metal/${ARCH}/rail_lib/common \
	-I${RODOS_SRC}/bare-metal/${ARCH}/common/bsp \
	-I${RODOS_SRC}/bare-metal/${ARCH}/hal-config \
	-I${RODOS_SRC}/bare-metal/${ARCH}/emlib/inc \
	-I${RODOS_SRC}/bare-metal/${ARCH}/common/drivers/used \
	-I${RODOS_SRC}/bare-metal/${ARCH}/config \
	-I${RODOS_SRC}/bare-metal/${ARCH}/rail-config \
	-I${RODOS_SRC}/bare-metal/${ARCH}/rail_lib/hal \
	-I${RODOS_SRC}/bare-metal/${ARCH}/emdrv/gpiointerrupt/inc/ "

export INCLUDES_TO_BUILD_LIB=" -I ${RODOS_SRC}/bare-metal-generic \
    -I ${RODOS_ARCH_SRC1} \
    -I ${RODOS_ARCH_SRC2} \
    -I ${RODOS_ARCH_SRC3} \
    -I ${RODOS_ARCH_SRC4} \
    -I ${RODOS_ARCH_SRC5} \
    -I ${RODOS_ARCH_SRC6} \
    -I ${RODOS_ARCH_SRC7} \
    -I ${RODOS_ARCH_SRC8} \
    -I ${RODOS_ARCH_SRC9} \
    -I ${RODOS_ARCH_SRC10} \
    -I ${RODOS_ARCH_SRC11} \
    -I ${RODOS_ARCH_SRC12} \
    -I ${RODOS_ARCH_SRC13} \
    -I ${RODOS_ARCH_SRC14} \
    -I ${RODOS_ARCH_SRC15} \
    -I ${RODOS_ARCH_SRC16} \
    -I ${RODOS_ARCH_SRC17} \
    -I ${RODOS_ARCH_SRC18} \
    -I ${RODOS_ARCH_SRC19} \
    -I ${RODOS_ARCH_SRC20} \
	-I ${RODOS_ARCH_SRC21} \
	-I ${RODOS_ARCH_SRC22} \
	-I ${RODOS_ARCH_SRC23} \
	-I ${RODOS_ARCH_SRC24} "

export CFLAGS_BASICS_COMMON=" -g3 -gdwarf-2 -DHSE_VALUE=${OSC_CLK} "
export CFLAGS_BASICS="${CFLAGS_BASICS_COMMON} -DCORTEXM3 -DCORTEXM3_EFR32_MICRO -DCORTEXM3_EFR32 -DEFR32FG1P -DEFR32FG1P133F256GM48 -DEFR32_SERIES1_CONFIG1_MICRO"
export HWCFLAGS=" -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -specs=nano.specs -specs=nosys.specs"
export LINKFLAGS=" -T${RODOS_ARCH_SRC1}/scripts/efr32fg1p.ld -I${RODOS_ARCH_SRC1}/scripts/ -nostartfiles -Xlinker --gc-sections -L${RODOS_LIBS}/${ARCH} -fno-unwind-tables -fno-asynchronous-unwind-tables -lm -lgcc -L${RODOS_SRC}/bare-metal/${ARCH} -lrodos -lrail_efr32xg1_gcc_release "
export CFLAGS=${CFLAGS}" ${CFLAGS_BASICS} ${HWCFLAGS} "

export ARM_TOOLS=""
export CPP_COMP="${ARM_TOOLS}arm-none-eabi-g++ "
export C_COMP="${ARM_TOOLS}arm-none-eabi-gcc " # only to compile BSP and Drivers from chip provider
export AR="${ARM_TOOLS}arm-none-eabi-ar "

