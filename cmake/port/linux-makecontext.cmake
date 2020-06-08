set(port_dir "bare-metal/linux-makecontext")
set(is_port_baremetal TRUE)

add_compile_options(-m32 -DIS_LINUX_MAKECONTEXT)
add_link_options(-m32)
