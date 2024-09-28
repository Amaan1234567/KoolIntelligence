
install(CODE "execute_process(COMMAND my_script.sh)")
include_directories(src/common/)
# Source files
set(SOURCES
    main.cpp
    ../common/logging.hpp
)

# Add the executable
add_executable(main ${SOURCES})