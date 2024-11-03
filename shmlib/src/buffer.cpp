/**
 * @file buffer.cpp
 * @brief Implementation of the Buffer class.
 */

#include <iostream>
#include "buffer.h"

bool Buffer::init(int shared_key)
{
    bool first_module = sharedMemory.init((void **)&bufferStruct, shared_key, sizeof(bufferStruct));
    if (first_module)
    {
        bufferStruct->initiated = false;
    }
    if (!bufferStruct)
    {
        std::cerr << "Shared memory initialization failed!\n";
        return false;
    }
    if (bufferStruct->initiated)
    {
        return true;
    }

    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&bufferStruct->mutex, &mutexattr);
    
    bufferStruct->initiated = true;
    return true;
}

void Buffer::write(const std::vector<uint8_t> &n_data)
{
    if (!bufferStruct || !bufferStruct->initiated)
    {
        std::cerr << "Buffer not initialized!\n";
        return;
    }
    pthread_mutex_lock(&bufferStruct->mutex);
    for (int i=0;i<32;i++)
    {
        bufferStruct->data[i] = n_data[i];
    }
    pthread_mutex_unlock(&bufferStruct->mutex);
}

std::vector<uint8_t> Buffer::read()
{
    if (!bufferStruct || !bufferStruct->initiated)
    {
        std::cerr << "Buffer not initialized!\n";
        return {};
    }
    std::vector<uint8_t> returnValue;
    returnValue.resize(32);
    for (int i=0;i<32;i++)
    {
        returnValue[i] = bufferStruct->data[i];
    }
    return returnValue;
}