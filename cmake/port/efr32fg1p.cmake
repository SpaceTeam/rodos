if(NOT board)
  message(FATAL_ERROR "Do not use efr32fg1p directly; instead, use a board port file.")
endif()

set(port_dir "bare-metal/efr32fg1p")
set(is_port_baremetal TRUE)

set(RODOS_DIR "${CMAKE_CURRENT_LIST_DIR}/../..")

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)


add_compile_definitions(CORTEXM3 CORTEXM3_EFR32_MICRO CORTEXM3_EFR32 ${SUB_ARCH_FLAGS})

set(compile_and_link_options -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16)
add_compile_options(-nostdlib -gdwarf-2)
add_compile_options(${compile_and_link_options} -mthumb)
add_link_options(${compile_and_link_options})
add_link_options(-T${RODOS_DIR}/src/bare-metal/efr32fg1p/scripts/${SUB_ARCH}.ld)
add_link_options(-nostartfiles -nostdlib -Xlinker --gc-sections -fno-unwind-tables -fno-asynchronous-unwind-tables -lm) # -l${RAIL_LIB})
link_libraries(${RAIL_LIB})
link_directories(${RODOS_DIR}/src/bare-metal/efr32fg1p/)


set(sources_to_add
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/emlib/src/*.c    
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/${SUB_ARCH_DIR}/Source/GCC/*.S
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/${SUB_ARCH_DIR}/Source/*.c
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/sleeptimer/src/*.c
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/common/src/*.c
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/common/bsp/*.c
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/common/drivers/used/*.c
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/rail_lib/hal/*.c
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/rail_lib/hal/efr32/*.c
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/rail-config/*.c
)

set(directories_to_include
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/CMSIS/Include
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/${SUB_ARCH_DIR}/Include
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/common/bsp
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/common/drivers/used
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/common/inc
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/config
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/emlib/inc
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/hal-config
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/rail-config
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/rail_lib/chip/efr32/efr32xg1x
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/rail_lib/common
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/rail_lib/hal
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/rail_lib/hal/efr32
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/sleeptimer/config
    ${RODOS_DIR}/src/bare-metal/efr32fg1p/sleeptimer/inc
)
