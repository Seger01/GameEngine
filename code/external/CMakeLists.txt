# Set options for SlikeNet
set(RAKNET_ENABLE_SAMPLES OFF CACHE BOOL "Generate RakNet sample projects if true." FORCE)
set(RAKNET_ENABLE_DLL OFF CACHE BOOL "Generate the DLL project if true." FORCE)
set(RAKNET_ENABLE_STATIC ON CACHE BOOL "Generate the static library project if true." FORCE)
set(RAKNET_GENERATE_INCLUDE_ONLY_DIR OFF CACHE BOOL "Setup a include/RakNet/ directory in which all the headers are copied." FORCE)

# Set options for SDL
set(SDL_SHARED ON CACHE BOOL "Build SDL as a shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Build SDL as a static library" FORCE)

# Audio settings
set(SDL_AUDIO ON CACHE BOOL "Enable audio support" FORCE)
set(SDL_ALSA ON CACHE BOOL "Enable ALSA audio support" FORCE)
set(SDL_PULSEAUDIO ON CACHE BOOL "Enable PulseAudio support" FORCE)
set(SDL_DISKAUDIO OFF CACHE BOOL "Enable disk audio support" FORCE)
set(SDL_DUMMYAUDIO OFF CACHE BOOL "Enable dummy audio support" FORCE)
set(SDL_OPENSLES OFF CACHE BOOL "Enable OpenSLES audio support" FORCE)
set(SDL_NAS OFF CACHE BOOL "Enable NAS audio support" FORCE)

# Video settings
set(SDL_VIDEO ON CACHE BOOL "Enable video support" FORCE)
set(SDL_RENDER ON CACHE BOOL "Enable render support" FORCE)
set(SDL_OPENGL ON CACHE BOOL "Enable OpenGL support" FORCE)
set(SDL_VULKAN OFF CACHE BOOL "Enable Vulkan support" FORCE)

# Input settings
set(SDL_INPUT_LINUX OFF CACHE BOOL "Enable Linux input support" FORCE)
set(SDL_JOYSTICK ON CACHE BOOL "Enable joystick support" FORCE)
set(SDL_HAPTIC ON CACHE BOOL "Enable haptic (force feedback) support" FORCE)

# Platform-specific options
set(SDL_DIRECTX OFF CACHE BOOL "Enable DirectX support (Windows)" FORCE)
set(SDL_WAYLAND ON CACHE BOOL "Enable Wayland support (Linux)" FORCE)
set(SDL_X11 ON CACHE BOOL "Enable X11 video support (Linux)" FORCE)

# Miscellaneous options
set(SDL_THREADS ON CACHE BOOL "Enable thread support" FORCE)
set(SDL_TIMERS OFF CACHE BOOL "Enable timer support" FORCE)
set(SDL_LOADSO ON CACHE BOOL "Enable shared object loading support" FORCE)
set(SDL_FILESYSTEM ON CACHE BOOL "Enable filesystem support" FORCE)
set(SDL_SENSOR OFF CACHE BOOL "Enable sensor support" FORCE)

# Disable building SDL test programs (if not needed)
set(SDL_TEST OFF CACHE BOOL "Build SDL test programs" FORCE)


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

# Set options for SDL_ttf
set(SDL2TTF_INSTALL OFF CACHE BOOL "Enable SDL2_ttf install target" FORCE)
set(SDL2TTF_DEPS_SHARED OFF CACHE BOOL "Load dependencies dynamically" FORCE)
set(SDL2TTF_VENDORED ON CACHE BOOL "Use vendored third-party libraries" FORCE)
set(SDL2TTF_SAMPLES OFF CACHE BOOL "Build the SDL2_ttf sample program(s)" FORCE) # Enable these if you need samples in poc for SDL_ttf
set(SDL2TTF_SAMPLES_INSTALL OFF CACHE BOOL "Install the SDL2_ttf sample program(s)" FORCE)
set(SDL2TTF_CMD OFF CACHE BOOL "Support an external text rendering program" FORCE)
set(SDL2TTF_BMP OFF CACHE BOOL "Support rendering text to BMP images" FORCE)
set(SDL2TTF_PNG OFF CACHE BOOL "Support rendering text to PNG images" FORCE)
set(SDL2TTF_TIF OFF CACHE BOOL "Support rendering text to TIFF images" FORCE)
set(SDL2TTF_WEBP OFF CACHE BOOL "Support rendering text to WEBP images" FORCE)
set(SDL2TTF_XCF OFF CACHE BOOL "Support rendering text to XCF images" FORCE)
set(SDL2TTF_XPM OFF CACHE BOOL "Support rendering text to XPM images" FORCE)
set(SDL2TTF_XV OFF CACHE BOOL "Support rendering text to XV images" FORCE)
set(SDL2TTF_FREETYPE ON CACHE BOOL "Use FreeType for font rendering" FORCE)
set(SDL2TTF_FREETYPE_SHARED OFF CACHE BOOL "Dynamically load FreeType" FORCE)

# Set options for BehaviorTree
set(BTCPP_EXAMPLES OFF CACHE BOOL "Build the BehaviorTree examples" FORCE)
set(BTCPP_UNIT_TESTS OFF CACHE BOOL "Build the BehaviorTree unit tests" FORCE)
set(BTCPP_NO_STD_OPTIONAL OFF CACHE BOOL "Disable usage of std::optional" FORCE)
set(BTCPP_NO_STD_VARIANT OFF CACHE BOOL "Disable usage of std::variant" FORCE)
set(BTCPP_NO_STD_ANY OFF CACHE BOOL "Disable usage of std::any" FORCE)
set(BTCPP_NO_COROUTINES OFF CACHE BOOL "Disable usage of coroutines" FORCE)
set(BTCPP_NO_EXCEPTIONS OFF CACHE BOOL "Disable usage of exceptions" FORCE)
set(BTCPP_NO_RTTI OFF CACHE BOOL "Disable usage of RTTI" FORCE)
set(BTCPP_NO_THREADS OFF CACHE BOOL "Disable usage of threads" FORCE)
set(BTCPP_USE_STD_FUNCTION OFF CACHE BOOL "Use std::function instead of std::function_view" FORCE)
set(BTCPP_GROOT_INTERFACE OFF CACHE BOOL "Enable Groot interface for BehaviorTree.CPP" FORCE)
set(BTCPP_SQLITE_LOGGING OFF CACHE BOOL "Enable SQLite logging for BehaviorTree.CPP" FORCE)
set(BTCPP_BUILD_TOOLS OFF CACHE BOOL "Build the BehaviorTree tools" FORCE)

# Set options for FreeType
set(FT_WITH_ZLIB ON CACHE BOOL "Enable Zlib support" FORCE)
set(FT_WITH_PNG OFF CACHE BOOL "Enable PNG support" FORCE)
set(FT_WITH_HARFBUZZ OFF CACHE BOOL "Enable HarfBuzz support" FORCE)
set(FT_WITH_BZIP2 OFF CACHE BOOL "Enable Bzip2 support" FORCE)
set(FT_WITH_BROTLI OFF CACHE BOOL "Enable Brotli support" FORCE)
set(FT_WITH_GZIP OFF CACHE BOOL "Enable Gzip support" FORCE)

# Add the external libraries
add_subdirectory(SLikeNet)
add_subdirectory(SDL)
add_subdirectory(SDL_mixer)
add_subdirectory(SDL_image)
add_subdirectory(SDL_ttf)
add_subdirectory(box2d)
add_subdirectory(nlohmann_json)
add_subdirectory(BehaviorTree)