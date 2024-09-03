# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Engine-BRAVO VERSION 0.1)

# Specify the C++ standard (e.g., C++11, C++14, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the header files
include_directories(${PROJECT_SOURCE_DIR}/include)

# Specify the source files
set(SOURCES
    src/main.cpp
    # Add more source files here
)

# Add the executable target
add_executable(engine_BRAVO ${SOURCES})
