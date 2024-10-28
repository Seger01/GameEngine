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

# Disable building tests for libraries.
set(BUILD_TESTING OFF CACHE BOOL "Disable building tests" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "Disable building tests" FORCE)
set(BUILD_SAMPLES OFF CACHE BOOL "Disable building samples" FORCE) # Enable these if you need samples in poc's
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable building examples" FORCE) # Enable these if you need examples in poc's

# Set options for SlikeNet
set(RAKNET_ENABLE_SAMPLES OFF CACHE BOOL "Generate RakNet sample projects if true." FORCE)
set(RAKNET_ENABLE_DLL OFF CACHE BOOL "Generate the DLL project if true." FORCE)
set(RAKNET_ENABLE_STATIC ON CACHE BOOL "Generate the static library project if true." FORCE)
set(RAKNET_GENERATE_INCLUDE_ONLY_DIR OFF CACHE BOOL "Setup a include/RakNet/ directory in which all the headers are copied." FORCE)

# Add the external libraries
add_subdirectory(external/SLikeNet)

# Include the header files
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/external/SLikeNet/Source)

# Automatically search for source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

#Exclude main.cpp from library sources
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$") #TODO add this in later when there are more files than just main.cpp

# Define the libraries to link against
set(LINK_LIBRARIES RakNetLibStatic ) #SDL2 SDL2_mixer SDL2_image tmxlite box2d

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
