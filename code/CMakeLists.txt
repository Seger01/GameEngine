# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Engine-BRAVO VERSION 0.1)

# Specify the C++ standard (e.g., C++11, C++14, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add debug options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode enabled")
    add_compile_options(-g -O0)
endif()

# Include the header files
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Automatically search for source files in the src directory
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Add the executable target
add_executable(engine_BRAVO ${SOURCES})

# =====================
# SDL2 Integration
# =====================

# Find SDL2
find_package(SDL2 REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

# Link SDL2 to the executable
target_link_libraries(engine_BRAVO ${SDL2_LIBRARIES})

# =====================
# Google Test Integration
# =====================

# FetchContent module to download GTest
include(FetchContent)

# Download and configure GTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip  # You can use the latest release
)

# Download GTest during configuration step
FetchContent_MakeAvailable(googletest)

# Enable testing in the project
enable_testing()

# Automatically search for test files in the test directory
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")

# Create an executable for tests
add_executable(tests ${TEST_SOURCES})

# Link GTest and GTest Main to the test executable
target_link_libraries(tests gtest gtest_main)

# Add test execution
add_test(NAME engine_BRAVO_tests COMMAND tests)

# Optionally, set up test include directories
# target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/inc)

