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

# Specify the source files
set(SOURCES
    src/main.cpp
    # Add more source files here
)

# Add the executable target
add_executable(engine_BRAVO ${SOURCES})

# =====================
# FMOD Integration
# =====================

# Path to the FMOD library and include directories
set(FMOD_DIR "~/fmod/fmodstudioapi20223linux/api/core")  # Replace this with the path where you unpacked FMOD
set(FMOD_INCLUDE_DIR "${FMOD_DIR}/inc")
set(FMOD_LIB_DIR "${FMOD_DIR}/lib/x86_64")

# Include the FMOD headers
include_directories(${FMOD_INCLUDE_DIR})

# Link FMOD library (.so for dynamic linking)
target_link_libraries(engine_BRAVO ${FMOD_LIB_DIR}/libfmod.so)

# Optionally, if you want static linking, use this:
# target_link_libraries(engine_BRAVO ${FMOD_LIB_DIR}/libfmod.a)

# =====================
# IrrKlang Integration
# =====================

# Path to the FMOD library and include directories
set(IRRKLANG_DIR "~/IrrKlang/irrKlang-64bit-1.6.0")  # Replace this with the path where you unpacked FMOD
set(IRRKLANG_INCLUDE_DIR "${IRRKLANG_DIR}/include")
set(IRRKLANG_LIB_DIR "${IRRKLANG_DIR}/bin/linux-gcc-64")

# Include the FMOD headers
include_directories(${IRRKLANG_INCLUDE_DIR})

# Link FMOD library (.so for dynamic linking)
target_link_libraries(engine_BRAVO ${IRRKLANG_LIB_DIR}/libIrrKlang.so)
# target_link_libraries(engine_BRAVO ${IRRKLANG_LIB_DIR}/ikpMP3.so) Does not work   

# Optionally, if you want static linking, use this:
# target_link_libraries(engine_BRAVO ${FMOD_LIB_DIR}/libfmod.a)

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
