target_sources(koolintelligence PRIVATE model_api.cpp transcribe_service.cpp tts_service.cpp)
set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
target_compile_definitions(koolintelligence PRIVATE PROJECT_DIR="${PROJECT_DIR}")
target_include_directories(koolintelligence PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(koolintelligence PRIVATE /usr/local/cuda/include)

add_subdirectory(thirdparty)
# Run external script (optional, only if needed)
find_program(OLLAMA_COMMAND ollama)

if(NOT OLLAMA_COMMAND)
    message(STATUS "Ollama not found. Please install it manually.")
    message(STATUS "Run the following commands in your terminal:")
    message(STATUS "1. chmod +x ${CMAKE_CURRENT_SOURCE_DIR}/ollama_install.sh")
    message(STATUS "2. ${CMAKE_CURRENT_SOURCE_DIR}/ollama_install.sh")
    message(FATAL_ERROR "After installing Ollama, please run CMake again.")
else()
    message(STATUS "Ollama found at: ${OLLAMA_COMMAND}")
endif()


set(SCRIPT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/create_tts_binary.sh")

add_custom_target(run_create_tts_binary
    COMMAND chmod +x ${SCRIPT_PATH}
    COMMAND ${SCRIPT_PATH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running create_tts_binary.sh"
    VERBATIM
)

# Install the script and set up the installation hook
install(CODE "
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build . --target run_create_tts_binary
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        RESULT_VARIABLE SCRIPT_RESULT
    )
    if(NOT SCRIPT_RESULT EQUAL 0)
        message(FATAL_ERROR \"Failed to run create_tts_binary.sh during installation\")
    endif()
")

