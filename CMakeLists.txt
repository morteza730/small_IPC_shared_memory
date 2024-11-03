cmake_minimum_required(VERSION 3.10)

project(shMem)

# Find Google Test
find_package(GTest REQUIRED)

# Add Google Test to the include directories
include_directories(${GTEST_INCLUDE_DIRS})

# Add Doxygen support
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()

add_subdirectory(shmlib)
add_subdirectory(supervisor)
add_subdirectory(tests)