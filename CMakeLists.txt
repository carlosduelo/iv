#
# Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
# Company: Cesvima, UPM
#
# Notes: Using Builyard https://github.com/Eyescale/Buildyard
#

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(IV)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common)

include(GitExternal)

set(LAST_RELEASE 0.1.0)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION_ABI 010)

include(Common)
include(Buildyard)

if(BUILDYARD_STOP)
    return()
endif()

# Options

set(IV_DESCRIPTION "Isosurface rendering")
set(PROJECT_INCLUDE_NAME iv)

if(RELEASE_VERSION)
    set(IV_DEFINITIONS -DIV_RELEASE)
endif()

include(GitTargets)
include(UpdateFile)

# Flags
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread" )

# Definitions
add_definitions(${IV_DEFINITIONS})

include(configure)

# Compile libraries
add_subdirectory(iv)

# Compile Utils
add_subdirectory(util)

# Compile Benchmarks
add_subdirectory(bench)

# Compile test
# add_subdirectory(test)

# Compile perf
#add_subdirectory(perf)
