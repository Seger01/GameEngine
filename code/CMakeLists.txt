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

# Set options for TmxLite
set(TMXLITE_STATIC_LIB OFF CACHE BOOL "Should tmxlite be built as a static or shared lib?" FORCE)
set(PROJECT_STATIC_RUNTIME OFF CACHE BOOL "Use statically linked standard/runtime libraries?" FORCE)
set(USE_RTTI OFF CACHE BOOL "Use run time type information?" FORCE)
set(USE_EXTLIBS OFF CACHE BOOL "Use external zlib, zstd and pugixml libraries instead of the included source?" FORCE)
set(USE_ZSTD OFF CACHE BOOL "Enable zstd compression? (Already set to true if USE_EXTLIBS is true)" FORCE)

# Set options for SlikeNet
set(RAKNET_ENABLE_SAMPLES OFF CACHE BOOL "Generate RakNet sample projects if true." FORCE)
set(RAKNET_ENABLE_DLL OFF CACHE BOOL "Generate the DLL project if true." FORCE)
set(RAKNET_ENABLE_STATIC ON CACHE BOOL "Generate the static library project if true." FORCE)
set(RAKNET_GENERATE_INCLUDE_ONLY_DIR OFF CACHE BOOL "Setup a include/RakNet/ directory in which all the headers are copied." FORCE)

# set options for SDL_mixer
set(SDL2MIXER_INSTALL OFF CACHE BOOL "Enable SDL2mixer install target" FORCE)
set(SDL2MIXER_DEPS_SHARED OFF CACHE BOOL "Default value for loading dependencies dynamically" FORCE)
set(SDL2MIXER_VENDORED ON CACHE BOOL "Use vendored third-party libraries" FORCE) # Set to ON if you enable almost any other option in this list
set(SDL2MIXER_SAMPLES OFF CACHE BOOL "Build the SDL2_mixer sample program(s)" FORCE) # Enable these if you need samples in poc for SDL_mixer
set(SDL2MIXER_SAMPLES_INSTALL OFF CACHE BOOL "Install the SDL2_mixer sample program(s)" FORCE)
set(SDL2MIXER_CMD OFF CACHE BOOL "Support an external music player" FORCE)
set(SDL2MIXER_FLAC OFF CACHE BOOL "Enable FLAC music" FORCE)
set(SDL2MIXER_FLAC_LIBFLAC OFF CACHE BOOL "Enable FLAC music using libFLAC" FORCE)
set(SDL2MIXER_FLAC_LIBFLAC_SHARED OFF CACHE BOOL "Dynamically load LIBFLAC" FORCE)
set(SDL2MIXER_FLAC_DRFLAC OFF CACHE BOOL "Enable FLAC music using drflac" FORCE)
set(SDL2MIXER_GME OFF CACHE BOOL "Support loading GME music via game-music-emu" FORCE)
set(SDL2MIXER_GME_SHARED OFF CACHE BOOL "Dynamically load libgme" FORCE)
set(SDL2MIXER_MOD OFF CACHE BOOL "Support loading MOD music" FORCE)
set(SDL2MIXER_MOD_MODPLUG OFF CACHE BOOL "Support loading MOD music via modplug" FORCE)
set(SDL2MIXER_MOD_MODPLUG_SHARED OFF CACHE BOOL "Dynamically load modplug" FORCE)
set(SDL2MIXER_MOD_XMP OFF CACHE BOOL "Support loading MOD music via libxmp" FORCE)
set(SDL2MIXER_MOD_XMP_LITE OFF CACHE BOOL "Use libxmp-lite instead of libxmp" FORCE)
set(SDL2MIXER_MOD_XMP_SHARED OFF CACHE BOOL "Dynamically load libxmp(-lite)" FORCE)
set(SDL2MIXER_MP3 OFF CACHE BOOL "Enable MP3 music" FORCE)
set(SDL2MIXER_MP3_MINIMP3 OFF CACHE BOOL "Support loading MP3 music via minimp3" FORCE)
set(SDL2MIXER_MP3_MPG123 OFF CACHE BOOL "Support loading MP3 music via MPG123" FORCE)
set(SDL2MIXER_MP3_MPG123_SHARED OFF CACHE BOOL "Dynamically load mpg123" FORCE)
set(SDL2MIXER_MIDI OFF CACHE BOOL "Enable MIDI music" FORCE)
set(SDL2MIXER_MIDI_FLUIDSYNTH OFF CACHE BOOL "Support FluidSynth MIDI output" FORCE)
set(SDL2MIXER_MIDI_FLUIDSYNTH_SHARED OFF CACHE BOOL "Dynamically load libfluidsynth" FORCE)

# Set options for SDL_image
set(SDL2IMAGE_INSTALL ON CACHE BOOL "Enable SDL2_image install target" FORCE)
set(SDL2IMAGE_DEPS_SHARED OFF CACHE BOOL "Load dependencies dynamically" FORCE)
set(SDL2IMAGE_VENDORED ON CACHE BOOL "Use vendored third-party libraries" FORCE)
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "Build the SDL2_image sample program(s)" FORCE) # Enable these if you need samples in poc for SDL_image
set(SDL2IMAGE_SAMPLES_INSTALL OFF CACHE BOOL "Install the SDL2_image sample program(s)" FORCE)
set(SDL2IMAGE_TESTS OFF CACHE BOOL "Build unit tests?" FORCE)
set(SDL2IMAGE_TESTS_INSTALL OFF CACHE BOOL "Install unit tests?" FORCE)
set(SDL2IMAGE_BACKEND_STB ON CACHE BOOL "Use stb_image for loading JPEG and PNG files" FORCE)
set(SDL2IMAGE_BACKEND_WIC OFF CACHE BOOL "Add WIC backend (Windows Imaging Component)" FORCE)
set(SDL2IMAGE_BACKEND_IMAGEIO OFF CACHE BOOL "Use native Mac OS X frameworks for loading images" FORCE)
set(SDL2IMAGE_AVIF OFF CACHE BOOL "Support loading AVIF images" FORCE)
set(SDL2IMAGE_BMP OFF CACHE BOOL "Support loading BMP images" FORCE)
set(SDL2IMAGE_GIF OFF CACHE BOOL "Support loading GIF images" FORCE)
set(SDL2IMAGE_JPG OFF CACHE BOOL "Support loading JPEG images" FORCE)
set(SDL2IMAGE_JXL OFF CACHE BOOL "Support loading JXL images" FORCE)
set(SDL2IMAGE_LBM OFF CACHE BOOL "Support loading LBM images" FORCE)
set(SDL2IMAGE_PCX OFF CACHE BOOL "Support loading PCX images" FORCE)
set(SDL2IMAGE_PNG ON CACHE BOOL "Support loading PNG images" FORCE)
set(SDL2IMAGE_PNM OFF CACHE BOOL "Support loading PNM images" FORCE)
set(SDL2IMAGE_QOI OFF CACHE BOOL "Support loading QOI images" FORCE)
set(SDL2IMAGE_SVG OFF CACHE BOOL "Support loading SVG images" FORCE)
set(SDL2IMAGE_TGA OFF CACHE BOOL "Support loading TGA images" FORCE)
set(SDL2IMAGE_TIF OFF CACHE BOOL "Support loading TIFF images" FORCE)
set(SDL2IMAGE_WEBP OFF CACHE BOOL "Support loading WEBP images" FORCE)
set(SDL2IMAGE_XCF OFF CACHE BOOL "Support loading XCF images" FORCE)
set(SDL2IMAGE_XPM OFF CACHE BOOL "Support loading XPM images" FORCE)
set(SDL2IMAGE_XV OFF CACHE BOOL "Support loading XV images" FORCE)
set(SDL2IMAGE_JPG_SAVE OFF CACHE BOOL "Add JPEG save support" FORCE)
set(SDL2IMAGE_PNG_SAVE OFF CACHE BOOL "Add PNG save support" FORCE)

# Set options for Box2D
set(BOX2D_SANITIZE OFF CACHE BOOL "Enable sanitizers for some builds" FORCE)
set(BOX2D_AVX2 OFF CACHE BOOL "Enable AVX2 (faster)" FORCE)
set(BOX2D_SAMPLES OFF CACHE BOOL "Build the Box2D samples" FORCE) # Enable these if you need samples in poc for Box2D
set(BOX2D_BENCHMARKS OFF CACHE BOOL "Build the Box2D benchmarks" FORCE)
set(BOX2D_DOCS OFF CACHE BOOL "Build the Box2D documentation" FORCE)
set(BOX2D_PROFILE OFF CACHE BOOL "Enable profiling with Tracy" FORCE)
set(BOX2D_VALIDATE OFF CACHE BOOL "Enable heavy validation" FORCE)
set(BOX2D_UNIT_TESTS OFF CACHE BOOL "Build the Box2D unit tests" FORCE)

# Add the external libraries
add_subdirectory(external/SLikeNet)
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_mixer)
add_subdirectory(external/SDL_image)
add_subdirectory(external/tmxlite/tmxlite)
add_subdirectory(external/box2d)
add_subdirectory(external/nlohmann_json)

# Include the header files
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/inc/Engine)
include_directories(${PROJECT_SOURCE_DIR}/inc/Components)
include_directories(${PROJECT_SOURCE_DIR}/inc/Global)
include_directories(${PROJECT_SOURCE_DIR}/inc/Input)
include_directories(${PROJECT_SOURCE_DIR}/inc/UI)
include_directories(${PROJECT_SOURCE_DIR}/inc/Save)

# Automatically search for source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Exclude main.cpp from library sources
# list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$") #TODO add this in later when there are more files than just main.cpp

# Define the libraries to link against
set(LINK_LIBRARIES RakNetLibStatic SDL2 SDL2_mixer SDL2_image tmxlite box2d nlohmann_json::nlohmann_json)

# Add the executable target
add_executable(engine_BRAVO ${SOURCES} ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Link libraries
target_link_libraries(engine_BRAVO PRIVATE ${LINK_LIBRARIES})

# Add shared and static library targets
add_library(engine_BRAVO_shared SHARED ${SOURCES})
add_library(engine_BRAVO_static STATIC ${SOURCES})

# Link SDL2 libraries to the shared and static libraries
target_link_libraries(engine_BRAVO_shared PRIVATE ${LINK_LIBRARIES})
target_link_libraries(engine_BRAVO_static PRIVATE ${LINK_LIBRARIES})

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

