set(CMAKE_AUTOMOC OFF)
cmake_minimum_required(VERSION 3.18)

message(STATUS "Starting CMake configuration...")

# Allow manual specification of CUDA compiler
set(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc" CACHE FILEPATH "CUDA compiler")

message(STATUS "Initializing project...")

message(STATUS "CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CXX Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CUDA Compiler: ${CMAKE_CUDA_COMPILER}")

#enable_language(CUDA)

#set(CMAKE_CUDA_ARCHITECTURES "60;70;75;80;86" CACHE STRING "CUDA architectures")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF)

message(STATUS "Finding SDL2...")
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    message(STATUS "Found SDL2 at ${SDL2_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "SDL2 not found")
endif()

message(STATUS "Finding CUDA...")
find_package(CUDA REQUIRED)
if(CUDA_FOUND)
    message(STATUS "Found CUDA ${CUDA_VERSION_STRING} at ${CUDA_TOOLKIT_ROOT_DIR}")
else()
    message(FATAL_ERROR "CUDA not found")
endif()

message(STATUS "Adding whisper.cpp subdirectory...")
add_subdirectory(whisper.cpp)

message(STATUS "Configuring CPU-only version...")
add_executable(cpuOnlyWhisper "${CMAKE_CURRENT_SOURCE_DIR}/whisper.cpp/examples/stream/stream.cpp")
target_sources(cpuOnlyWhisper PRIVATE 
    whisper.cpp/examples/common-sdl.cpp 
    whisper.cpp/examples/common.cpp
)
target_include_directories(cpuOnlyWhisper PRIVATE
    whisper.cpp/include
    whisper.cpp/spm-headers
    whisper.cpp/examples
    ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(cpuOnlyWhisper PRIVATE SDL2 whisper)
target_compile_definitions(cpuOnlyWhisper PRIVATE WHISPER_USE_OPENVINO)

message(STATUS "Configuring CUDA version...")
add_executable(cudaWhisper "${CMAKE_CURRENT_SOURCE_DIR}/whisper.cpp/examples/stream/stream.cpp")
target_sources(cudaWhisper PRIVATE 
    whisper.cpp/examples/common-sdl.cpp 
    whisper.cpp/examples/common.cpp
)
target_include_directories(cudaWhisper PRIVATE
    whisper.cpp/include
    whisper.cpp/spm-headers
    whisper.cpp/examples
    ${SDL2_INCLUDE_DIRS}
    ${CUDA_INCLUDE_DIRS}
)
target_link_libraries(cudaWhisper PRIVATE SDL2 whisper ${CUDA_LIBRARIES})

target_compile_definitions(cudaWhisper PRIVATE GGML_USE_CUBLAS)

message(STATUS "Configuring installation...")
install(TARGETS cpuOnlyWhisper cudaWhisper ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

message(STATUS "CMake configuration completed.")