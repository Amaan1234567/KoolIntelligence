cmake_minimum_required(VERSION 3.20)

# Project definition
project(KoolIntelligence VERSION 0.0.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-unused-function -Wno-ignored-qualifiers")

# Enable debug mode
set(CMAKE_AUTOMOC ON)
option(ENABLE_DEBUG "Enable debug mode" ON)

if(ENABLE_DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

# Set common flags
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Add subdirectories
add_subdirectory(src/ui)
add_subdirectory(src/common)
add_subdirectory(src/model_backend)
add_subdirectory(src/screenshot)

#include(KDEClangFormat)
#file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.hpp *.c)
#kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

