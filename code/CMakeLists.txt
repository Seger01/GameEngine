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

# Execute the Install.sh script
execute_process(
    COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/install ./Install.sh
    RESULT_VARIABLE install_result
)

if(NOT install_result EQUAL 0)
    message(FATAL_ERROR "Install.sh script failed with exit code ${install_result}")
endif()

# Set SDL2MIXER_MIDI option
option(SDL2MIXER_MIDI "Enable MIDI support in SDL2_mixer" OFF)
set(SDL2MIXER_MIDI OFF)

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

# Disable building tests for SLikeNet
set(BUILD_TESTING OFF CACHE BOOL "Disable building tests" FORCE)

# Add the external libraries
add_subdirectory(external/SLikeNet)
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_mixer)
add_subdirectory(external/SDL_image) 
add_subdirectory(external/tmxlite/tmxlite)
add_subdirectory(external/box2d)


# Specify include directories of the external libraries
# include_directories(${Slikenet_INCLUDE_DIRS})
# include_directories(${PROJECT_SOURCE_DIR}/external/tmxlite/tmxlite/include)
# include_directories(${PROJECT_SOURCE_DIR}/external/box2d/include)
# include_directories(${PROJECT_SOURCE_DIR}/external/SDL_image)


# Add the executable target
add_executable(engine_BRAVO ${SOURCES})

# Link SDL2 libraries
target_link_libraries(engine_BRAVO PRIVATE RakNetLibStatic SDL2 SDL2_mixer SDL2_image tmxlite box2d) 

# =====================
# Google Test Integration
# =====================

# Option to enable or disable Google Test
option(ENABLE_GTEST "Enable Google Test" OFF)

if(ENABLE_GTEST)
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
endif()