set(board discovery)

set(SUB_ARCH stm32f40x)
set(OSC_CLK 8000000)
include(${CMAKE_CURRENT_LIST_DIR}/stm32f4.cmake)

# only for platform-parameter.h
list(APPEND directories_to_include "src/bare-metal/stm32f4/platform-parameter/discovery")
