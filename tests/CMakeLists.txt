cmake_minimum_required(VERSION 3.10)

# Add the test executable
add_executable(BufferTest testBuffer.cpp)
target_link_libraries(BufferTest SHLib GTest::gtest GTest::gtest_main)

add_executable(NotifierTest testNotifier.cpp)
target_link_libraries(NotifierTest SHLib GTest::gtest GTest::gtest_main)

# Add the test to the test suite
add_test(NAME BufferTest COMMAND BufferTest)
add_test(NAME NotifierTest COMMAND NotifierTest)