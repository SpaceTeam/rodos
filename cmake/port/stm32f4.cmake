set(port_dir "bare-metal/stm32f4")
set(is_port_baremetal TRUE)

set(RODOS_DIR "${CMAKE_CURRENT_LIST_DIR}/../..")

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

set(OSC_CLK 8000000)

add_compile_definitions(HSE_VALUE=${OSC_CLK} STM32F40_41xxx USE_STM32_DISCOVERY USE_STDPERIPH_DRIVER)

add_compile_options(-nostdlib -gdwarf-2)
add_compile_options(-mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16)
add_link_options(-T${RODOS_DIR}/src/bare-metal/stm32f4/scripts/stm32_flash.ld)
add_link_options(-nostartfiles -nodefaultlibs -nostdlib -Xlinker --gc-sections -fno-unwind-tables -fno-asynchronous-unwind-tables)

set(sources_to_add
    ${RODOS_DIR}/src/bare-metal/stm32f4/STM32F4xx_StdPeriph_Driver/src/*.c
    ${RODOS_DIR}/src/bare-metal/stm32f4/startup/*.c
    ${RODOS_DIR}/src/bare-metal/stm32f4/startup/*.S)

set(directories_to_include
    ${RODOS_DIR}/src/bare-metal/stm32f4/STM32F4xx_StdPeriph_Driver/inc
    ${RODOS_DIR}/src/bare-metal/stm32f4/CMSIS/Device/ST/STM32F4xx/Include
    ${RODOS_DIR}/src/bare-metal/stm32f4/CMSIS/Include)
