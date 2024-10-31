﻿#pragma once

#include "SharedMemory.h"
#include <thread>
#include <stdint.h>
#include <pthread.h>

struct BufferStruct
{
    uint8_t data;
    bool initiated = false;
    pthread_mutex_t mutex;
};

class Buffer
{
public:
    Buffer(){}
    bool init(int shared_key);
    void write(uint8_t n_data);
    uint8_t read();

private:
    SharedMemory sharedMemory;
    BufferStruct *bufferStruct;
};
