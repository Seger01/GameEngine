# FindGMock.cmake
# Locate GMock
# This module defines
#  GMock_INCLUDE_DIR, where to find gmock/gmock.h, etc.
#  GMock_LIBRARIES, the libraries to link against to use GMock.
#  GMock::GMock, the GMock target

find_package(GTest REQUIRED)

find_path(GMock_INCLUDE_DIR
  NAMES gmock/gmock.h
  HINTS ${GTest_INCLUDE_DIRS}
)

find_library(GMock_LIBRARIES
  NAMES gmock
  HINTS ${GTest_LIBRARIES}
)

find_library(GMock_Main_LIBRARIES
  NAMES gmock_main
  HINTS ${GTest_LIBRARIES}
)

if (GMock_INCLUDE_DIR AND GMock_LIBRARIES)
  set(GMock_FOUND TRUE)
endif ()

if (GMock_FOUND)
  if (NOT TARGET GMock::GMock)
    add_library(GMock::GMock UNKNOWN IMPORTED)
    set_target_properties(GMock::GMock PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${GMock_INCLUDE_DIR}"
      IMPORTED_LOCATION "${GMock_LIBRARIES}"
    )
  endif ()

  if (NOT TARGET GMock::Main)
    add_library(GMock::Main UNKNOWN IMPORTED)
    set_target_properties(GMock::Main PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${GMock_INCLUDE_DIR}"
      IMPORTED_LOCATION "${GMock_Main_LIBRARIES}"
    )
  endif ()
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMock REQUIRED_VARS GMock_INCLUDE_DIR GMock_LIBRARIES GMock_Main_LIBRARIES)