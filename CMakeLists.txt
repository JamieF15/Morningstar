cmake_minimum_required(VERSION 3.14)
project(Morningstar LANGUAGES CXX)

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
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # only affects MSVC
FetchContent_MakeAvailable(googletest)

# ---------------------------
# Source files
# ---------------------------
file(GLOB SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# ---------------------------
# Test files
# ---------------------------
file(GLOB TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
)

# ---------------------------
# Executable for tests
# ---------------------------
add_executable(Morningstar_tests
    ${SRC_FILES}
    ${TEST_FILES}
)

# Add include/ to headers
target_include_directories(Morningstar_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# MSVC: ensure correct runtime
if(MSVC)
    set_target_properties(Morningstar_tests PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL"
    )
endif()

# Link GoogleTest (not gtest_main, since you provide main())
target_link_libraries(Morningstar_tests PRIVATE gtest)

# Register test with CTest
add_test(NAME AllTests COMMAND $<TARGET_FILE:Morningstar_tests>)
