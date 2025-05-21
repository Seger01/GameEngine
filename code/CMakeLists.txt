# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(GameEngine VERSION 0.1)

set(PROJECT_EXECUTABLE ${PROJECT_NAME}_exec)
set(TESTS ${PROJECT_NAME}_tests)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Option to enable or disable building tests
option(ENABLE_TESTS "Enable building tests" OFF)

if(ENABLE_TESTS)
    # Set coverage compile and link flags
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
set(BUILD_SAMPLES OFF CACHE BOOL "Disable building samples" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable building examples" FORCE)

# Add the external libraries
add_subdirectory(external)

# Automatically search for source files
file(GLOB_RECURSE LIBRARY_SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Create a library from the library sources
add_library(${PROJECT_NAME} STATIC ${LIBRARY_SOURCES})

# Automatically search for subdirectories in the include directory
file(GLOB SUBDIRS RELATIVE ${PROJECT_SOURCE_DIR}/inc ${PROJECT_SOURCE_DIR}/inc/*)

# Add the root include directory
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc)

# # Add each subdirectory under the include directory
# foreach(subdir ${SUBDIRS})
#     if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/inc/${subdir})
#         target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc/${subdir})
#     endif()
# endforeach()

# Automatically search for subdirectories in external dependencies and add them
file(GLOB EXTERNAL_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/external/box2d/include
    ${PROJECT_SOURCE_DIR}/external/nlohmann_json/include
    ${PROJECT_SOURCE_DIR}/external/SDL/include
    # ${PROJECT_SOURCE_DIR}/external/SDL_image/include
    ${PROJECT_SOURCE_DIR}/external/SDL_mixer/include
    ${PROJECT_SOURCE_DIR}/external/renderer/inc
    # ${PROJECT_SOURCE_DIR}/external/SDL_ttf
    ${PROJECT_SOURCE_DIR}/external/SLikeNet/Source
)

# Include each external library directory
foreach(ext_dir ${EXTERNAL_INCLUDE_DIRS})
    if(IS_DIRECTORY ${ext_dir})
        target_include_directories(${PROJECT_NAME} PUBLIC ${ext_dir})
    endif()
endforeach()

# Define the libraries to link against
set(LINK_LIBRARIES SDL2 SDL2_mixer RakNetLibStatic box2d nlohmann_json::nlohmann_json renderer)

# Link libraries to library
target_link_libraries(${PROJECT_NAME} PUBLIC ${LINK_LIBRARIES})

if(ENABLE_TESTS)
    # Add the test subdirectory
    add_subdirectory(test)
endif()

# Add a custom target for Doxygen
find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    # Paths
    set(DOXYFILE_PATH "${CMAKE_SOURCE_DIR}/../Doxyfile")
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/doxygen")

    # Create the output directory if it doesn't exist
    file(MAKE_DIRECTORY "${DOXYGEN_OUTPUT_DIR}")

    # Create a custom target to invoke the custom command
    add_custom_target( Doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_PATH}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/..
        COMMENT "Generating API documentation with Doxygen"
    )
else()
    message(FATAL_ERROR "Doxygen is required but was not found on the system.")
endif()
