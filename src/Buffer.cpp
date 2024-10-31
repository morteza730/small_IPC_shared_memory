#include <iostream>
#include "Buffer.h"

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

void Buffer::write(uint8_t n_data)
{
    pthread_mutex_lock(&bufferStruct->mutex);
    bufferStruct->data = n_data;
    pthread_mutex_unlock(&bufferStruct->mutex);
}

uint8_t Buffer::read()
{
    return bufferStruct->data;
}
