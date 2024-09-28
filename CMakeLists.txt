cmake_minimum_required(VERSION 3.20)

# Project definition
project(KoolIntelligence VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable debug mode
option(ENABLE_DEBUG "Enable debug mode" ON)

if(ENABLE_DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set common flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fexceptions")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")





# Add subdirectories
add_subdirectory(src/ui)
add_subdirectory(src/common)
add_subdirectory(src/model_backend)
