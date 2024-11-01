#include "sharedMemory.h"

#include <errno.h>
#include <iostream>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

// ------------------------------ init memory ------------------------------
bool SharedMemory::init(void **memPtr, int key, size_t size)
{

    if (!allocate(key, size))
    {
        perror("allocation error!\n");
        *memPtr = nullptr;
        return false;
    }
    if (!attach())
    {
        perror("attach error!\n");
        *memPtr = nullptr;
        return false;
    }

    if (memPtr != nullptr)
        *memPtr = _memPtr;
    if (get_attached_number() == 1)
    {
        return true;
    }
    else
        return false;
}

bool SharedMemory::attach()
{
    _memPtr = shmat(shmid, NULL, 0);
    if (_memPtr == (void *)-1)
        return false;
    return true;
}

bool SharedMemory::allocate(int key, int size)
{
    shmid = shmget(key, size, IPC_CREAT | 0666);
    if (shmid == -1)
        return false;
    return true;
}

bool SharedMemory::free()
{
    if (shmdt(_memPtr) == -1)
    {
        perror("detachment error!\n"); 
        return false;
    }
    if (shmctl(shmid, IPC_RMID, 0) == -1)
    {
        perror("removing identifier error!\n");
        return false;
    }
    return true;
}

int SharedMemory::get_attached_number()
{
    shmid_ds info;
    if (shmctl(shmid, IPC_STAT, &info) == -1)
    {
        perror("getting ipc status error!\n");
        return -1;
    }
    return info.shm_nattch;
}

void *SharedMemory::get() { return _memPtr; } // Get shared memory pointer