cmake_minimum_required(VERSION 3.14)
project(Morningstar2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# ---------------------------
# Fetch GoogleTest
# ---------------------------
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# Use dynamic runtime to match MSVC
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ---------------------------
# Source files (exclude tests)
# ---------------------------
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/returns_analyser.cpp
    # Add other .cpp files here as needed
)

# ---------------------------
# Test files
# ---------------------------
set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

# ---------------------------
# Create test executable
# ---------------------------
add_executable(Morningstar_tests ${SOURCES} ${TEST_SOURCES})

# On MSVC, use dynamic runtime
if(MSVC)
    set_target_properties(Morningstar_tests PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL"
    )
endif()

# Link GoogleTest (not gtest_main because we provide main())
target_link_libraries(Morningstar_tests PRIVATE gtest)

# Register with CTest
add_test(NAME AllTests COMMAND $<TARGET_FILE:Morningstar_tests>)
