cmake_minimum_required(VERSION 3.10)

project(shMem)

# Find Google Test
find_package(GTest REQUIRED)

# Add Google Test to the include directories
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(shmlib)
add_subdirectory(supervisor)
add_subdirectory(tests)