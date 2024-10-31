#ifndef __NSR_IPC_NOTIFIER_H__
#define __NSR_IPC_NOTIFIER_H__

#include "SharedMemory.h"
#include <thread>
#include <stdint.h>
#include <pthread.h>

struct NotifData  {

    pthread_cond_t notif;
    pthread_mutex_t mutex;

    bool initiated = false;
    uint64_t trigger_counter;
};

class InterProccNotifier {

public:
    InterProccNotifier(){;}
    bool init(int shared_key);

    uint64_t notify();
    uint64_t wait(double timeout_sec=0.0);
    uint64_t waitIfNoNewNotif(uint64_t &last_trigger_number, double timeout_sec=0.0);

private:
    SharedMemory shmSeg;
    NotifData* notifData;
};

#endif //__NSR_IPC_NOTIFIER_H__