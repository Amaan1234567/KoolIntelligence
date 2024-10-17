set(CMAKE_AUTOMOC OFF)
set(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc" CACHE FILEPATH "CUDA compiler")

set(BUILD_SHARED_LIBS OFF)
find_package(SDL2 REQUIRED)
#enable_language(CUDA)

add_subdirectory(whisper.cpp)

message(STATUS "Configuring CPU-only version...")
add_executable(cpuOnlyWhisper)
target_sources(cpuOnlyWhisper PRIVATE 
    whisper.cpp/examples/stream/stream.cpp
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
add_executable(cudaOnlyWhisper)
target_sources(cudaOnlyWhisper PRIVATE 
    whisper.cpp/examples/stream/stream.cpp
    whisper.cpp/examples/common-sdl.cpp 
    whisper.cpp/examples/common.cpp
)
target_include_directories(cudaOnlyWhisper PRIVATE
    whisper.cpp/include
    whisper.cpp/spm-headers
    whisper.cpp/examples
    ${SDL2_INCLUDE_DIRS}
    ${CUDA_INCLUDE_DIRS}
)
target_link_libraries(cudaOnlyWhisper PRIVATE SDL2 whisper ${CUDA_LIBRARIES})

target_compile_definitions(cudaOnlyWhisper PRIVATE GGML_USE_CUBLAS)
install(TARGETS cpuOnlyWhisper cudaOnlyWhisper ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


add_executable(voice_out ${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/examples/main/main.cpp)
target_sources(voice_out PRIVATE

${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/encodec.cpp/ggml/src/ggml-alloc.c
${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/encodec.cpp/ggml/src/ggml-backend.c

)

add_library(ggml_bark)

target_sources(ggml_bark PRIVATE

${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/encodec.cpp/ggml/src/ggml-alloc.c
${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/encodec.cpp/ggml/src/ggml-backend.c

)
target_include_directories(ggml_bark PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/encodec.cpp/ggml/include/
)

add_library(bark)
target_sources(bark PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/bark.cpp
${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/encodec.cpp/encodec.cpp
)
target_include_directories(bark PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/spm-headers/ 
)

add_library(common)
target_sources(common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/examples/common.cpp)
target_include_directories(common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/bark.cpp/examples/)
target_link_libraries(voice_out ggml_bark bark common)

