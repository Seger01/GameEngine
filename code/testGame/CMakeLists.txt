# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

set(PROJECT_EXECUTABLE ${PROJECT_NAME}_exec)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the header files
include_directories(${PROJECT_SOURCE_DIR}/testGame/inc)

# Automatically search for source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/testGame/src/*.cpp
)

# Add the executable target
add_executable(${PROJECT_EXECUTABLE} ${SOURCES})

# Add the library as a dependency
add_dependencies(${PROJECT_EXECUTABLE} ${PROJECT_NAME})

# Link library to the executable
target_link_libraries(${PROJECT_EXECUTABLE} PUBLIC ${PROJECT_NAME})

# Add custom target to run the main program
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_EXECUTABLE}
    DEPENDS ${PROJECT_EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Running the main program"
)

# Add custom target to run Valgrind
add_custom_target(run_valgrind
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_EXECUTABLE}
    DEPENDS ${PROJECT_EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Running Valgrind to check for memory leaks"
)