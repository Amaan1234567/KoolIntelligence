target_sources(koolintelligence PRIVATE file_search.cpp)
target_include_directories(koolintelligence PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.hpp *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})