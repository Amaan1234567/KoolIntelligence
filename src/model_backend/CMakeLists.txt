target_sources(koolintelligence PRIVATE model_api.cpp)
target_include_directories(koolintelligence PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(thirdparty)
# Run external script (optional, only if needed)
install(CODE "execute_process(COMMAND ollama.sh)")