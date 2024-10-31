#include "InterProccessNotifier.h"
#include "Utility.h"
#include <iostream>
#include <sys/time.h>

#ifndef EOK
#define EOK 0
#endif

#define NANO_COEF 1000UL*1000UL*1000UL 

// ======================================================= INIT =======================================================
bool InterProccNotifier::init(int shared_key) {

    bool first_module = shmSeg.init((void**)&notifData, shared_key, sizeof(NotifData));     // SharedMemory::init(memPtr, key, size)
    if (first_module) {
        notifData->initiated = false;
        notifData->trigger_counter = 0;
    }
    if (!notifData)              {std::cerr << "Shared memory initialization failed!\n";  return false;}         
    if (notifData->initiated)    {return true;}    // notifier was initiated before!

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

// ======================================================= NOTIFY =======================================================
uint64_t InterProccNotifier::notify() {

    pthread_mutex_lock(&notifData->mutex);
    notifData->trigger_counter++;
    pthread_cond_broadcast(&notifData->notif);      // pthread_cond_signal    -> to trigger first waited proccess
    //                                            // pthread_cond_broadcast -> to trigger   all waited proccesses
    pthread_mutex_unlock(&notifData->mutex);
    return notifData->trigger_counter;
}

// ======================================================= WAIT =======================================================
uint64_t InterProccNotifier::wait(double timeout_sec) {

    int ret;

    struct timeval now;
    struct timespec timeToWait;
    gettimeofday(&now, NULL);

    int time_in_sec = now.tv_sec + (int)(timeout_sec);
    int time_in_miliSec = (timeout_sec-(int)(timeout_sec)) * 1000UL;
    uint64_t time_in_nanoSec = (now.tv_usec + time_in_miliSec * 1000UL) * 1000UL;
    if(time_in_nanoSec > NANO_COEF) { time_in_sec++; time_in_nanoSec -= NANO_COEF;}

    timeToWait.tv_sec = time_in_sec;
    timeToWait.tv_nsec = time_in_nanoSec;

    pthread_mutex_lock(&notifData->mutex);

    if(EQUALS(timeout_sec, 0 , 1e-6))                               // No time-out
        ret = pthread_cond_wait(&notifData->notif, &notifData->mutex);
    else                                                            // With time-out
        ret = pthread_cond_timedwait(&notifData->notif, &notifData->mutex, &timeToWait);

    pthread_mutex_unlock(&notifData->mutex);

    if(ret == ETIMEDOUT)     return 0; 

    return notifData->trigger_counter;
}

// ======================================================================================================================
uint64_t InterProccNotifier::waitIfNoNewNotif(uint64_t &last_trigger_number, double timeout_sec) {    

    int ret;
    struct timeval now;
    struct timespec timeToWait;
    gettimeofday(&now, NULL);

    int time_in_sec = now.tv_sec + (int)(timeout_sec);
    int time_in_miliSec = (timeout_sec-(int)(timeout_sec)) * 1000UL;
    uint64_t time_in_nanoSec = (now.tv_usec + time_in_miliSec * 1000UL) * 1000UL;
    if(time_in_nanoSec > NANO_COEF) { time_in_sec++; time_in_nanoSec -= NANO_COEF;}

    timeToWait.tv_sec = time_in_sec;
    timeToWait.tv_nsec = time_in_nanoSec;

    pthread_mutex_lock(&notifData->mutex);

    if(last_trigger_number == notifData->trigger_counter){
        if(EQUALS(timeout_sec, 0, 1e-6))
            ret = pthread_cond_wait(&notifData->notif, &notifData->mutex);
        else 
            ret = pthread_cond_timedwait(&notifData->notif, &notifData->mutex, &timeToWait);
    }
    last_trigger_number = notifData->trigger_counter;

    pthread_mutex_unlock(&notifData->mutex);

    if(ret == ETIMEDOUT)     return 0; 

    return notifData->trigger_counter;
}   
// ======================================================================================================================