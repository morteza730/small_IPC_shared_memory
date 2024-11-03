/**
 * @file notifier.h
 * @brief Header file for the Notifer class.
 */

#pragma once

#include "sharedMemory.h"
#include <thread>
#include <stdint.h>
#include <pthread.h>

/**
 * @struct NotifData
 * @brief Structure to hold notification data and synchronization primitives.
 */
struct NotifData
{
    pthread_cond_t notif; /**< Condition variable for notification. */
    pthread_mutex_t mutex; /**< Mutex for synchronization. */
    bool initiated = false; /**< Flag to indicate if the notifier is initiated. */
};

/**
 * @class Notifer
 * @brief Class to manage notification operations.
 */
class Notifer
{
public:
    Notifer() {}
    /**
     * @brief Initialize the notifier with a shared memory key.
     * @param shared_key The shared memory key.
     * @return True if initialization is successful, false otherwise.
     */
    bool init(int shared_key);

    /**
     * @brief Notify waiting threads.
     */
    void notify();

    /**
     * @brief Wait for a notification.
     * @return True if wait is successful, false otherwise.
     */
    bool wait();

private:
    SharedMemory sharedMemory; /**< Shared memory instance. */
    NotifData *notifData; /**< Pointer to the notification data structure. */
};