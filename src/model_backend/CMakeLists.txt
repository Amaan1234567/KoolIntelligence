include_directories(${CMAKE_SOURCE_DIR}/)


# Set the path for whisper.cpp and ggml
set(WHISPER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/whisper.cpp)
set(GGML_DIR ${WHISPER_DIR}/ggml)

# Add whisper.cpp and ggml source files
set(WHISPER_SOURCES
    ${WHISPER_DIR}/src/whisper.cpp
    ${GGML_DIR}/src/ggml.c
)

# Add the stream example source files
set(STREAM_SOURCES
    ${WHISPER_DIR}/examples/stream/stream.cpp
    ${WHISPER_DIR}/examples/common.cpp
    ${WHISPER_DIR}/examples/common-sdl.cpp
)

# Create a library target for whisper
add_library(whisper_backend STATIC ${WHISPER_SOURCES})

# Include directories for whisper and ggml libraries
target_include_directories(whisper_backend PUBLIC 
    ${WHISPER_DIR}/include
    ${GGML_DIR}/include
)

# Create the stream executable
add_executable(stream ${STREAM_SOURCES})

# Include directories for the stream executable
target_include_directories(stream PUBLIC 
    ${WHISPER_DIR}/include
    ${WHISPER_DIR}/examples
    ${GGML_DIR}/include
)

# Find and link SDL2 for stream
find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    target_include_directories(stream PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_libraries(stream PRIVATE ${SDL2_LIBRARIES} whisper_backend)
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()

# Ensure CUDA support is included if necessary
if (GGML_USE_CUBLAS)
    find_package(CUDA REQUIRED)
    target_compile_definitions(whisper_backend PUBLIC GGML_USE_CUBLAS)
    target_link_libraries(whisper_backend ${CUDA_LIBRARIES})
endif()

# Link the main executable with whisper_backend
target_link_libraries(koolintelligence PRIVATE whisper_backend)

# Include whisper headers for the main executable
target_include_directories(koolintelligence PRIVATE 
    ${WHISPER_DIR}/include
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
