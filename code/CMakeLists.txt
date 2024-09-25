# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Engine-BRAVO VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add a CMake option to enable or disable debug mode
option(ENABLE_DEBUG "Enable debug mode" ON)

# Set the build type based on the ENABLE_DEBUG option
if(ENABLE_DEBUG)
    message("Debug mode enabled")
    set(CMAKE_BUILD_TYPE Debug)
    add_compile_options(-g -O0)
else()
    set(CMAKE_BUILD_TYPE Release)
    add_compile_options(-O3)  # Optimization flag for Release build
endif()

# Set architecture-specific flags
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

# Ensure position-independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Specify output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib_external)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Execute the Install.sh script
execute_process(
    COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/install ./Install.sh
    RESULT_VARIABLE install_result
)

if(NOT install_result EQUAL 0)
    message(FATAL_ERROR "Install.sh script failed with exit code ${install_result}")
endif()

# Disable building tests for libraries
set(BUILD_TESTING OFF CACHE BOOL "Disable building tests" FORCE)
set(SDL_TEST OFF CACHE BOOL "Disable building SDL2 test library" FORCE)

# Set SDL2MIXER_MIDI option
option(SDL2MIXER_MIDI "Enable MIDI support in SDL2_mixer" OFF)
set(SDL2MIXER_MIDI OFF)

# Add the external libraries
add_subdirectory(external/SLikeNet)
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_mixer)
add_subdirectory(external/SDL_image)
add_subdirectory(external/tmxlite/tmxlite)
add_subdirectory(external/box2d)

# Include the header files
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Automatically search for source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Exclude main.cpp from library sources
# list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$") #TODO add this in later when there are more files than just main.cpp

# Define the libraries to link against
set(LINK_LIBRARIES RakNetLibStatic SDL2 SDL2_mixer SDL2_image tmxlite box2d)

# Add the executable target
add_executable(engine_BRAVO ${SOURCES} ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Link SDL2 libraries
target_link_libraries(engine_BRAVO PRIVATE ${LINK_LIBRARIES})

# Add shared and static library targets
add_library(engine_BRAVO_shared SHARED ${SOURCES})
add_library(engine_BRAVO_static STATIC ${SOURCES})

# Link SDL2 libraries to the shared and static libraries
target_link_libraries(engine_BRAVO_shared PRIVATE ${LINK_LIBRARIES})
target_link_libraries(engine_BRAVO_static PRIVATE ${LINK_LIBRARIES})

# =====================
# SDL2 and SDL2_image Integration
# =====================

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(engine_BRAVO ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
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

# Automatically search for test files in the test directory
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")
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

# Optionally, set up test include directories
# target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/inc)

    # Add test execution
    add_test(NAME engine_BRAVO_tests COMMAND tests)

    # Optionally, set up test include directories
    #target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/inc)
endif()
