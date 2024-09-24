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


if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "i686")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a")
endif()

# Include the header files
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Automatically search for source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add the external libraries
add_subdirectory(external/slikenet)
#add_subdirectory(external/tinyxml2-9.0.0)

# Specify include directories of the external libraries
include_directories(${Slikenet_INCLUDE_DIRS})

# Add the executable target
add_executable(engine_BRAVO ${SOURCES})

# Link SDL2 libraries
target_link_libraries(engine_BRAVO RakNetLibStatic ${SDL2_LIBRARIES} )

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

# Add the test source files
set(TEST_SOURCES
    test/test_main.cpp    # Your test source files
    # Add more test files here
)

# Create an executable for tests
add_executable(tests ${TEST_SOURCES})

# Link GTest and GTest Main to the test executable
target_link_libraries(tests gtest gtest_main)

# Add test execution
add_test(NAME engine_BRAVO_tests COMMAND tests)

# Optionally, set up test include directories
#target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/inc)
