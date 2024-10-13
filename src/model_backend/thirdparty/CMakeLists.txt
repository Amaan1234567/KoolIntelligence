set(CMAKE_AUTOMOC OFF)
set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
set(BUILD_SHARED_LIBS OFF)
#set(WHISPER_OPENVINO ON)
#set(GGML_CUDA ON)

find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    message(STATUS "Found SDL2 at ${SDL2_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "SDL2 not found")
endif()


add_executable(cpuOnlyWhisper "${CMAKE_CURRENT_SOURCE_DIR}/whisper.cpp/examples/stream/stream.cpp")
add_executable(cudaOnlyWhisper "${CMAKE_CURRENT_SOURCE_DIR}/whisper.cpp/examples/stream/stream.cpp")

target_sources(cpuOnlyWhisper PRIVATE whisper.cpp/examples/common-sdl.cpp whisper.cpp/examples/common.cpp)
target_sources(cudaOnlyWhisper PRIVATE whisper.cpp/examples/common-sdl.cpp whisper.cpp/examples/common.cpp)

#add_subdirectory(whisper.cpp)

target_compile_definitions(cpuOnlyWhisper PRIVATE -DWHISPER_OPENVINO=1)
target_compile_definitions(cudaOnlyWhisper PRIVATE -DGGML_CUDA=1)

target_include_directories(cpuOnlyWhisper PUBLIC whisper.cpp/include)
target_include_directories(cpuOnlyWhisper PUBLIC whisper.cpp/spm-headers)
target_include_directories(cpuOnlyWhisper PUBLIC whisper.cpp/examples)
target_include_directories(cpuOnlyWhisper PUBLIC  ${SDL2_INCLUDE_DIRS})



target_include_directories(cudaOnlyWhisper PUBLIC whisper.cpp/include)
target_include_directories(cudaOnlyWhisper PUBLIC whisper.cpp/spm-headers)
target_include_directories(cudaOnlyWhisper PUBLIC whisper.cpp/examples)
target_include_directories(cudaOnlyWhisper PUBLIC ${SDL2_INCLUDE_DIRS})


target_link_libraries(cpuOnlyWhisper PRIVATE SDL2 whisper)
target_link_libraries(cudaOnlyWhisper PRIVATE SDL2 whisper)

install(TARGETS cpuOnlyWhisper ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(TARGETS cudaOnlyWhisper ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})