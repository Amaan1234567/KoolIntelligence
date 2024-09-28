# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(KoolIntelligence VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for header files
add_library(model_backend STATIC model_api.cpp)  # Include other source files if necessary
target_include_directories(model_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Run external script (optional, only if needed)
install(CODE "execute_process(COMMAND my_script.sh)")
