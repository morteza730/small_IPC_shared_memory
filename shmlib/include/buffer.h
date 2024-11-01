#pragma once

#include "sharedMemory.h"
#include <thread>
#include <stdint.h>
#include <pthread.h>
#include <vector>

struct BufferStruct
{
    uint8_t data[32];
    bool initiated = false;
    pthread_mutex_t mutex;
};

class Buffer
{
public:
    Buffer(){}
    bool init(int shared_key);
    void write(const std::vector<uint8_t> &n_data);
    std::vector<uint8_t> read();

private:
    SharedMemory sharedMemory;
    BufferStruct *bufferStruct;
};
