# =====================
# Google Test Integration
# =====================

cmake_minimum_required(VERSION 3.10)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Option to enable or disable building tests
option(ENABLE_TESTS "Enable building tests" OFF)

if(ENABLE_TESTS)
    # Set the CMAKE_MODULE_PATH to include the directory containing FindGMock.cmake
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

    # Find GoogleTest and GoogleMock packages
    find_package(GTest REQUIRED)
    find_package(GMock REQUIRED)

    # Automatically search for test files in the test directory
    file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")

    # Add the test executable
    add_executable(${TESTS} ${TEST_SOURCES})

    # Add dependencies to the main library
    add_dependencies(${TESTS} ${PROJECT_NAME})

    # Link GoogleTest, GoogleMock libraries and the main library
    target_link_libraries(${TESTS} PRIVATE GTest::GTest GMock::GMock pthread ${PROJECT_NAME})

    # Add custom target to run the tests
    add_custom_target(run_tests
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TESTS}
        DEPENDS ${TESTS}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Running the tests"
    )
endif()
