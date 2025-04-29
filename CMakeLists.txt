cmake_minimum_required(VERSION 3.14)
project(xnode VERSION 1.0.0 LANGUAGES CXX 
        DESCRIPTION "Lightweight, header-only C++ library for dynamically typed values"
        HOMEPAGE_URL "https://github.com/vpiotr/xnode")

# Options
option(XNODE_BUILD_TESTS "Build xnode tests" ON)
option(XNODE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
    if(XNODE_WARNINGS_AS_ERRORS)
        add_compile_options(/WX)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(XNODE_WARNINGS_AS_ERRORS)
        add_compile_options(-Werror)
    endif()
endif()

# Add include directory
include_directories(include)

# Create xnode library (header-only)
add_library(xnode INTERFACE)
target_include_directories(xnode INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Add aliases to support find_package() and add_subdirectory() usage
add_library(xnode::xnode ALIAS xnode)

# Install targets
install(TARGETS xnode
    EXPORT xnodeTargets
    INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/ DESTINATION include)

# Generate and install package config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/xnode-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/xnode-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/xnode-config.cmake"
    INSTALL_DESTINATION lib/cmake/xnode
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/xnode-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/xnode-config-version.cmake"
    DESTINATION lib/cmake/xnode
)

install(EXPORT xnodeTargets
    FILE xnode-targets.cmake
    NAMESPACE xnode::
    DESTINATION lib/cmake/xnode
)

# Export package for use from the build tree
export(EXPORT xnodeTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/xnode-targets.cmake"
    NAMESPACE xnode::
)

# Build tests
if(XNODE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()