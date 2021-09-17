set(board discovery_f429)

set(SUB_ARCH stm32f429)
set(OSC_CLK 8000000)
include(${CMAKE_CURRENT_LIST_DIR}/stm32f4.cmake)

# only for platform-parameter.h
list(APPEND directories_to_include "${RODOS_DIR}/src/bare-metal/stm32f4/platform-parameter/discovery_f429")
