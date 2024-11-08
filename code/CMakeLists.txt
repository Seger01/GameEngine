# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Engine_BRAVO VERSION 0.1)

set(PROJECT_EXECUTABLE ${PROJECT_NAME}_exec)
set(TESTS ${PROJECT_NAME}_tests)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add a CMake option to enable or disable debug mode
option(ENABLE_DEBUG "Enable debug mode" OFF)

# Option to enable or disable building tests
option(ENABLE_TESTS "Enable building tests" OFF)

# Set the build type based on the ENABLE_DEBUG option
if(ENABLE_DEBUG)
    message("Debug mode enabled")
    set(CMAKE_BUILD_TYPE Debug)
    add_compile_options(-g -O0)
else()
    set(CMAKE_BUILD_TYPE Release)
    add_compile_options(-O3)  # Optimization flag for Release build
endif()

if(ENABLE_TESTS)
    #Set coverage compile and link flags
    add_compile_options(--coverage)
    add_link_options(--coverage)
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

# Specify output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib_external)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Disable building tests for libraries.
set(BUILD_TESTING OFF CACHE BOOL "Disable building tests" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "Disable building tests" FORCE)
set(BUILD_SAMPLES OFF CACHE BOOL "Disable building samples" FORCE) # Enable these if you need samples in poc's
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable building examples" FORCE) # Enable these if you need examples in poc's

# Add the external libraries
add_subdirectory(external)

# Include the header files
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/inc/Engine)
include_directories(${PROJECT_SOURCE_DIR}/inc/Components)
include_directories(${PROJECT_SOURCE_DIR}/inc/Global)
include_directories(${PROJECT_SOURCE_DIR}/inc/Input)
include_directories(${PROJECT_SOURCE_DIR}/inc/UI)
include_directories(${PROJECT_SOURCE_DIR}/inc/Save)
include_directories(${PROJECT_SOURCE_DIR}/inc/Network)

include_directories(${PROJECT_SOURCE_DIR}/external/SLikeNet/Source)


# Automatically search for source files
file(GLOB_RECURSE LIBRARY_SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Create a library from the library sources
add_library(${PROJECT_NAME} STATIC ${LIBRARY_SOURCES})

# Define the libraries to link against
set(LINK_LIBRARIES RakNetLibStatic SDL2 SDL2_mixer SDL2_image SDL2_ttf box2d nlohmann_json::nlohmann_json behaviortree_cpp)

# Link libraries to library
target_link_libraries(${PROJECT_NAME} PUBLIC ${LINK_LIBRARIES})

# Add the testGame subdirectory
add_subdirectory(testGame)

if(ENABLE_TESTS)
    # Add the testGame subdirectory
    add_subdirectory(test)
endif()