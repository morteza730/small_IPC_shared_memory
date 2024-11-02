#pragma once

#include "sharedMemory.h"
#include <thread>
#include <stdint.h>
#include <pthread.h>

struct NotifData
{
    pthread_cond_t notif;
    pthread_mutex_t mutex;
    bool initiated = false;
};

class Notifer
{

public:
    Notifer() {}
    bool init(int shared_key);
    void notify();
    bool wait();

private:
    SharedMemory sharedMemory;
    NotifData *notifData;
};