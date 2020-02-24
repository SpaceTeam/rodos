set(port_dir "bare-metal/sf2")
set(is_port_baremetal TRUE)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

set(LINK_SCRIPT debug-in-microsemi-smartfusion2-envm_w_ddr.ld)

add_compile_definitions(MICROSEMI_STDIO_THRU_UART MULTICAST)

add_compile_options(-fsigned-char -ffunction-sections)
add_compile_options(-fdata-sections --specs=nano.specs)
add_compile_options(-mcpu=cortex-m3 -mthumb)

add_link_options(-T${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/CMSIS/startup_gcc/${LINK_SCRIPT})
add_link_options(-nostartfiles -nostdlib)
add_link_options(-fno-unwind-tables -fno-asynchronous-unwind-tables -ffunction-sections -g3)
add_link_options(-fdata-sections -Xlinker --gc-sections -Wl,-Map,sf2.map)

set(sources_to_add
    ${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/rodos/*.cpp
    ${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/rodos/*.c
    ${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/rodos/hal/*.cpp
    ${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/drivers/*.c
    ${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/CMSIS/*.c
    ${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/CMSIS/startup_gcc/*.S
    ${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/drivers_config/sys_config/*.c
    ${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/hal/CortexM3/*.c)

set(directories_to_include
    ${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/rodos
    ${CMAKE_SOURCE_DIR}/src/bare-metal/sf2/CMSIS)
