install(CODE "execute_process(COMMAND my_script.sh)")
target_sources(koolintelligence
    PRIVATE
    model_api.cpp
)
target_include_directories(koolintelligence PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})