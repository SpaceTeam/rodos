add_executable(flyout flyout.cpp)
target_link_libraries(flyout PRIVATE rodos)

add_custom_command(
    TARGET flyout
    POST_BUILD
    COMMAND "${CMAKE_OBJCOPY}" -O binary flyout flyout.bin
    BYPRODUCTS flyout.bin
    COMMENT "Calling objcopy to generate flashable .bin file."
    VERBATIM
)
