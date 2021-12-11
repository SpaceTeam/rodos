include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Name of the package to use when calling find_package(<package>) in consumer code
set(package rodos)

# Copy all necessary header files to the install include directory
set(rodos_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/rodos)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/api/
    DESTINATION ${rodos_INSTALL_INCLUDEDIR}/api
    FILES_MATCHING PATTERN "*.h"
)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/src/${port_dir}/
    DESTINATION ${rodos_INSTALL_INCLUDEDIR}/src/${port_dir}
    FILES_MATCHING PATTERN "*.h"
)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/support/support-libs/
    DESTINATION ${rodos_INSTALL_INCLUDEDIR}/support/support-libs
    FILES_MATCHING PATTERN "*.h"
)
if (is_port_baremetal)
    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/src/bare-metal-generic/
        DESTINATION ${rodos_INSTALL_INCLUDEDIR}/src/bare-metal-generic
        FILES_MATCHING PATTERN "*.h"
    )
endif ()

# Define how to install which targets and create an export. This is where the include directories
# for the exported targets are defined. For rodos only the api subfolder is necessary since it
# contains rodos.h and that's the only header file consumer code shall include.
install(TARGETS rodos_rodos EXPORT rodosTargets
    INCLUDES
        DESTINATION ${rodos_INSTALL_INCLUDEDIR}/api
)

write_basic_package_version_file("${package}ConfigVersion.cmake"
    VERSION ${rodos_VERSION}
    COMPATIBILITY SameMajorVersion
)

set(
    rodos_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(rodos_INSTALL_CMAKEDIR)

install(FILES cmake/install-config.cmake
    DESTINATION ${rodos_INSTALL_CMAKEDIR}
    RENAME "${package}Config.cmake"
)

install(FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION ${rodos_INSTALL_CMAKEDIR}
)

# Generate and install a CMake file with code for installing targets from the given export
install(EXPORT rodosTargets
    NAMESPACE rodos::
    DESTINATION ${rodos_INSTALL_CMAKEDIR}
)
