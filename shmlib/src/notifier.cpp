#include "notifier.h"
#include <iostream>


// ======================================================= INIT =======================================================
bool Notifer::init(int shared_key)
{

    bool first_module = sharedMemory.init((void **)&notifData, shared_key, sizeof(NotifData));
    if (first_module)
    {
        notifData->initiated = false;
    }
    if (!notifData)
    {
        std::cerr << "Shared memory initialization failed!\n";
        return false;
    }
    if (notifData->initiated)
    {
        return true;
    }
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&notifData->mutex, &mutexattr);

    pthread_condattr_t condattr;
    pthread_condattr_init(&condattr);
    pthread_condattr_setpshared(&condattr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&notifData->notif, &condattr);

    notifData->initiated = true;
    return true;
}

void Notifer::notify()
{
    pthread_mutex_lock(&notifData->mutex);
    pthread_cond_broadcast(&notifData->notif); 
    pthread_mutex_unlock(&notifData->mutex);
}

bool Notifer::wait(double timeout_sec)
{
    pthread_mutex_lock(&notifData->mutex);
    pthread_cond_wait(&notifData->notif, &notifData->mutex);
    pthread_mutex_unlock(&notifData->mutex);
    return true;
}