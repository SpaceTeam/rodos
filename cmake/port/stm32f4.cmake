if(NOT board)
  message(FATAL_ERROR "Do not use stm32f4 directly; instead, use a board port file.")
endif()

set(port_dir "bare-metal/stm32f4")
set(is_port_baremetal TRUE)

set(RODOS_DIR "${CMAKE_CURRENT_LIST_DIR}/../..")

#################################################
# cross compiler toolchain
#################################################
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

if(MINGW OR CYGWIN OR WIN32)
    set(UTIL_SEARCH_CMD where)
elseif(UNIX OR APPLE)
    set(UTIL_SEARCH_CMD which)
endif()

set(TOOLCHAIN_PREFIX arm-none-eabi-)

execute_process(
    COMMAND ${UTIL_SEARCH_CMD} ${TOOLCHAIN_PREFIX}gcc
    OUTPUT_VARIABLE BINUTILS_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Without that flag CMake is not able to pass test compilation check
set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=nosys.specs")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump CACHE INTERNAL "objdump tool")
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


#################################################
# platform specific configuration
#################################################

# From here, we can have different sub achictures. We use the
# STM32F407VGT6 [168MHz, 192KByte SRAM == Discovery board] as default

if(NOT DEFINED SUB_ARCH)
    set(SUB_ARCH stm32f40x)
    MESSAGE("DID NOT GET SUB_ARCH Override, using default: ${SUB_ARCH}")

    # possible values are:
    # -> stm32f429   (STM32F429Discovery)
    # -> stm32f427   (STM32F427ZI)
    # -> stm32f411   (STM32F411CE)
    # -> stm32f40x   (STM32F4Discovery, MINI-M4)
    # -> stm32f401   (STM32F401 NUCLEO)
else()
    MESSAGE("SUB_ARCH used ${SUB_ARCH}")
endif()

if(NOT DEFINED OSC_CLK)
    set(OSC_CLK 8000000)
    MESSAGE("DID NOT GET OSC_CLK Override, using default: ${OSC_CLK}")

    # possible values are:
    # -> 8000000     (STM32F4Discovery, STM32F429Discovery, STM32F411Discovery)
    # -> 16000000    (MINI-M4, SKITH)
    # -> 24000000    (PX4FLOW, HoneyCloud)
else()
    MESSAGE("OSC_CLK used ${OSC_CLK}")
endif()

if(NOT DEFINED linker_script)
    if(SUB_ARCH STREQUAL "stm32f429")
        set(linker_script ${RODOS_DIR}/src/${port_dir}/scripts/stm32f429_flash.ld)
    elseif (SUB_ARCH STREQUAL "stm32f427")
        set(linker_script ${RODOS_DIR}/src/${port_dir}/scripts/stm32f427zi_flash.ld)
    elseif (SUB_ARCH STREQUAL "stm32f411")
        set(linker_script ${RODOS_DIR}/src/${port_dir}/scripts/stm32f411xe_flash.ld)
    elseif (SUB_ARCH STREQUAL "stm32f401")
        set(linker_script ${RODOS_DIR}/src/${port_dir}/scripts/stm32f401_flash.ld)
    else()
        set(linker_script ${RODOS_DIR}/src/${port_dir}/scripts/stm32_flash.ld)
    endif()
    MESSAGE("DID NOT GET linker_script Override, using default: ${linker_script}")
else()
    MESSAGE("Linker script used ${linker_script}")
endif()


add_compile_definitions(HSE_VALUE=${OSC_CLK} USE_STDPERIPH_DRIVER)
set(compile_and_link_options -mcpu=cortex-m4 -mfpu=fpv4-sp-d16)
add_compile_options(-gdwarf-2 -mthumb -g3)
add_link_options(-Wl,-T${linker_script})
add_link_options(
    -nostartfiles -nodefaultlibs -nostdlib
    -Xlinker --gc-sections
    -fno-unwind-tables -fno-asynchronous-unwind-tables)

if(SUB_ARCH STREQUAL "stm32f40x")
    list(APPEND compile_and_link_options -mfloat-abi=softfp)
    add_compile_definitions(STM32F40_41xxx USE_STM32_DISCOVERY)
elseif(SUB_ARCH STREQUAL "stm32f429")
    list(APPEND compile_and_link_options -mfloat-abi=hard)
    add_compile_definitions(STM32F429_439xx USE_STM32F429I_DISCO)
elseif(SUB_ARCH STREQUAL "stm32f427")
    list(APPEND compile_and_link_options -mfloat-abi=hard)
    add_compile_definitions(STM32F427_437xx)
elseif(SUB_ARCH STREQUAL "stm32f411")
    list(APPEND compile_and_link_options -mfloat-abi=hard)
    add_compile_definitions(STM32F411xE USE_STM32_DISCOVERY HSE_STARTUP_TIMEOUT=10000000)
elseif(SUB_ARCH STREQUAL "stm32f401")
    list(APPEND compile_and_link_options -mfloat-abi=hard)
    add_compile_definitions(STM32F401xx)
endif()

add_compile_options(${compile_and_link_options})
add_link_options(${compile_and_link_options})


set(sources_to_add "${RODOS_DIR}/src/bare-metal/stm32f4/startup/*.c")
if(SUB_ARCH STREQUAL "stm32f411")
  list(APPEND sources_to_add "${RODOS_DIR}/src/bare-metal/stm32f4/startup/stm32f411xe/*.S")
else()
  list(APPEND sources_to_add "${RODOS_DIR}/src/bare-metal/stm32f4/startup/stm32f4xx/*.S")
endif()
list(APPEND sources_to_add "${RODOS_DIR}/src/bare-metal/stm32f4/STM32F4xx_StdPeriph_Driver/src/*.c")


set(directories_to_include
    ${RODOS_DIR}/src/bare-metal/stm32f4/STM32F4xx_StdPeriph_Driver/inc
    ${RODOS_DIR}/src/bare-metal/stm32f4/CMSIS/Device/ST/STM32F4xx/Include
    ${RODOS_DIR}/src/bare-metal/stm32f4/CMSIS/Include)

set(libraries_to_link
    c   # the math library may need some stuff from newlib aka libc (like errno)
    m
    gcc
)
