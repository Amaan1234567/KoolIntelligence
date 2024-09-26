cmake_minimum_required(VERSION 3.20)
project(KoolIntelligence VERSION 0.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_DEBUG "Enable debug mode" OFF)

if(ENABLE_DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

add_subdirectory(src/ui)
add_subdirectory(src/common)
