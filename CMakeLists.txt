cmake_minimum_required(VERSION 3.18)

# Project name and version
project(Random 
    VERSION 1.0.0 
    DESCRIPTION "Random number generator library using L'Ecuyer's combined LCG"
    LANGUAGES C
)

# Only build shared libs by default on Windows
if(WIN32)
    option(BUILD_SHARED_LIBS "Build shared libraries" ON)
endif()

# Source files
set(LIB_SOURCES src/random.c)
set(LIB_HEADERS include/random.h)

# ============================================
# Library targets
# ============================================

# Shared library
add_library(random SHARED ${LIB_SOURCES})
target_sources(random PRIVATE ${LIB_HEADERS})

target_include_directories(random
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(random PUBLIC c_std_11)
target_compile_definitions(random PRIVATE RANDOM_BUILD_SHARED)

set_target_properties(random PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

# Static library
add_library(random_static STATIC ${LIB_SOURCES})
target_sources(random_static PRIVATE ${LIB_HEADERS})

target_include_directories(random_static
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(random_static PUBLIC c_std_11)

set_target_properties(random_static PROPERTIES
    OUTPUT_NAME random
    POSITION_INDEPENDENT_CODE ON
)

# Alias for internal use
add_library(Random::random ALIAS random)
add_library(Random::random_static ALIAS random_static)

# ============================================
# Example application
# ============================================

add_executable(app examples/main.c)
target_link_libraries(app PRIVATE Random::random)

# ============================================
# Test suite
# ============================================

add_executable(test_random tests/test_random.c)
target_link_libraries(test_random PRIVATE Random::random_static)

enable_testing()
add_test(NAME RandomTests COMMAND test_random)
set_tests_properties(RandomTests PROPERTIES
    LABELS "unit"
    TIMEOUT 30
)

# ============================================
# Installation
# ============================================

include(GNUInstallDirs)

install(TARGETS random random_static
    EXPORT RandomTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${LIB_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT RandomTargets
    FILE RandomTargets.cmake
    NAMESPACE Random::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Random
)

# Generate and install package config files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/RandomConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/RandomConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Random
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/RandomConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/RandomConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/RandomConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Random
)


