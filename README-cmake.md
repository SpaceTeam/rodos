How to build RODOS with CMake
=============================

RODOS supports different target platforms, which require different compilation configuration.
The configuration for all the ports is encapsulated in separate cmake files at `cmake/port`.
There, port configuration files such as `posix.cmake`, `linux-x86.cmake`, and `stm32f4.cmake` exist.
In order to enable a port, we make use of a special feature of CMake: the [`CMAKE_TOOLCHAIN_FILE` variable](https://cmake.org/cmake/help/v3.15/variable/CMAKE_TOOLCHAIN_FILE.html).
If this variable is set on the command line when `cmake` is called, the content of the supplied file is executed before the main `CMakeLists.txt` is processed.

Generate Make Files
-------------------

```shell script
$ mkdir build
$ cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=cmake/port/posix.cmake ..
```

First, create a folder, where all the make files and the compiled files should be stored.
If you want to use multiple ports at once, it is sensible to create separate folders for them, e.g. `build-linux` and `build-stm32f4`.
When calling cmake, the appropriate port `.cmake` file must be passed via the variable `CMAKE_TOOLCHAIN_FILE`.

Compile RODOS
-------------
```shell script
# make sure to be in the build directory
$ make -j rodos
```

This commands builds the RODOS library, which is then placed at `build/librodos.a`

Compile RODOS' Support Lib
--------------------------
```shell script
# make sure to be in the build directory
$ make -j support-lib
```

This commands builds the RODOS' support library, which is then placed at `build/support/support_libs/libsupport-lib.a`

Compile Tutorials
-----------------
For some of the tutorials, there are ready compilation targets in CMake.
In order to enable them, you must set the `TUTORIALS` variable to `ON`: 

```shell script
$ cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=cmake/port/posix.cmake -DTUTORIALS=ON ..
```

After that, several new targets are available, which can be compiled by using their names, e.g.

```shell script
$ make -j helloworld
```

The compiled executables are then available under `build/tutorials`.

Create Own Application Using Cmake
----------------------------------

There are two ways of compiling an own application that makes use of RODOS: either simply create an own CMake target as usual and link RODOS and the support lib if needed, or use our predefined simple macro, does these steps in only one call.

```shell script
include(rodos/cmake/executable.cmake)

add_rodos_executable(my-application my-source.cpp)
```

does the same thing as

```shell script
add_executable(my-application my-source.cpp)
target_link_libraries(my-application PUBLIC rodos support-lib)
```

After those lines you can use all the features of CMake.
