/**
 * @file buffer.h
 * @brief Header file for the Buffer class.
 */

#pragma once

#include "sharedMemory.h"
#include <thread>
#include <stdint.h>
#include <pthread.h>
#include <vector>

/**
 * @struct BufferStruct
 * @brief Structure to hold buffer data and synchronization primitives.
 */
struct BufferStruct
{
    uint8_t data[32]; /**< Data array of size 32. */
    bool initiated = false; /**< Flag to indicate if the buffer is initiated. */
    pthread_mutex_t mutex; /**< Mutex for synchronization. */
};

/**
 * @class Buffer
 * @brief Class to manage shared memory buffer operations.
 */
class Buffer
{
public:
    Buffer(){}
    /**
     * @brief Initialize the buffer with a shared memory key.
     * @param shared_key The shared memory key.
     * @return True if initialization is successful, false otherwise.
     */
    bool init(int shared_key);

    /**
     * @brief Write data to the buffer.
     * @param n_data The data to write.
     */
    void write(const std::vector<uint8_t> &n_data);

    /**
     * @brief Read data from the buffer.
     * @return The data read from the buffer.
     */
    std::vector<uint8_t> read();

private:
    SharedMemory sharedMemory; /**< Shared memory instance. */
    BufferStruct *bufferStruct; /**< Pointer to the buffer structure. */
};