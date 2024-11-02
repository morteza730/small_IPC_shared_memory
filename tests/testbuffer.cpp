#include <gtest/gtest.h>
#include "buffer.h"

TEST(BufferTest, Initialization) {
    Buffer buffer;
    EXPECT_TRUE(buffer.init(0x3333));
}

TEST(BufferTest, WriteRead) {
    Buffer buffer;
    buffer.init(0x3333);

    std::vector<uint8_t> data = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
    buffer.write(data);

    std::vector<uint8_t> readData = buffer.read();
    EXPECT_EQ(data, readData);
}