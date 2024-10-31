
#pragma once

#include <cstddef>
#include <stdbool.h>

class SharedMemory
{
public:
    bool init(void** memPtr, // shared memory pointer address or NULL
                int key,
                size_t size
    );                       // init() returns "true" if allocation happens, so memory initialization is needed

    void* get();             //Get shared memory pointer

    bool free();             // Use "free" if you need to release the memory(for all the programs)

    ~SharedMemory(){}        // Don't Remove shared Memory in deconstructor,
                             // It will release the memory for other attached programs and cause them to crash

    SharedMemory() : shmid(-1), _memPtr(nullptr){}

private:
    int shmid;
    void* _memPtr;
        
    bool attach();
    bool allocate(int key, int size);
    
    int  get_attached_number();

};

