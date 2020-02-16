cmake_minimum_required(VERSION 3.10)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

add_compile_options(-g -Wall -pedantic -Wno-long-long -Wno-cpp -fno-exceptions -O0 -fno-exceptions)
add_compile_options( $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti> $<$<COMPILE_LANGUAGE:CXX>:-std=c++14>)

project(rodos C CXX ASM)

if (COVERAGE)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "Coverage enabled, setting build type to Debug")
  endif()
  include(cmake/CodeCoverage.cmake)
  append_coverage_compiler_flags()
endif()

#___________________________________________________________________
file(GLOB_RECURSE independent_sources
    src/independent/*.cpp
    support/support-libs/*.cpp)

add_library(rodos STATIC ${independent_sources})

file(GLOB port_sources
    src/${port_dir}/*.cpp
    src/${port_dir}/*.c
    src/${port_dir}/hal/*.cpp
    src/${port_dir}/*.S)
target_sources(rodos PRIVATE ${port_sources})

target_include_directories(rodos PUBLIC
    api
    api/hal
    src/${port_dir}
    src/${port_dir}/include
    support/support-libs)


#___________________________________________________________________
if (is_port_baremetal)
  file(GLOB baremetal_generic_sources src/bare-metal-generic/*.cpp)
  target_sources(rodos PRIVATE ${baremetal_generic_sources})
  target_include_directories(rodos PUBLIC
      src/bare-metal-generic
      src/bare-metal-generic/ip/lwip/include)
endif ()


#___________________________________________________________________
#    optional extra files which are port dependent. See cmake/port/xxx.cmake
#___________________________________________________________________
if (sources_to_add)
  file(GLOB files ${sources_to_add})
  target_sources(rodos PRIVATE ${files})
endif ()

if (directories_to_include)
  target_include_directories(rodos PUBLIC ${directories_to_include})
endif ()

if (libraries_to_link)
  target_link_libraries(rodos PUBLIC ${libraries_to_link})
endif ()

if (EXECUTABLE)
  include(cmake/executable.cmake)
  add_subdirectory(tutorials)
endif ()


if (COVERAGE)
  setup_lcov_coverage(
    NAME coverage
    LCOV_ARGS "--no-external")
    message("Generate coverage report with 'make coverage'")
endif()
enable_testing()
add_subdirectory(test-suite)
