#pragma once

#include <cstddef>
#include <stdbool.h>

class SharedMemory
{
public:
    // init() returns "true" if allocation happens, so memory initialization is needed
    bool init(void **memPtr,
              int key,
              size_t size);
    // release the memory(for all processes)
    bool free();

    ~SharedMemory() {}

    SharedMemory() : shmid(-1), _memPtr(nullptr) {}

private:
    int shmid;
    void *_memPtr;
    void *get();
    bool attach();
    bool allocate(int key, int size);
    int get_attached_number();
};
