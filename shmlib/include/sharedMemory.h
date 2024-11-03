/**
 * @file sharedMemory.h
 * @brief Header file for the SharedMemory class.
 */

#pragma once

#include <cstddef>
#include <stdbool.h>

/**
 * @class SharedMemory
 * @brief Class to manage shared memory operations.
 */
class SharedMemory
{
public:
    /**
     * @brief Initialize shared memory.
     * @param memPtr Pointer to the memory location.
     * @param key Shared memory key.
     * @param size Size of the shared memory.
     * @return True if initialization is successful, false otherwise.
     */
    bool init(void **memPtr, int key, size_t size);

    /**
     * @brief Release the shared memory.
     * @return True if release is successful, false otherwise.
     */
    bool free();

    ~SharedMemory() {}

    SharedMemory() : shmid(-1), _memPtr(nullptr) {}

private:
    int shmid; /**< Shared memory ID. */
    void *_memPtr; /**< Pointer to the shared memory. */

    /**
     * @brief Get the shared memory pointer.
     * @return The shared memory pointer.
     */
    void *get();

    /**
     * @brief Attach to the shared memory.
     * @return True if attach is successful, false otherwise.
     */
    bool attach();

    /**
     * @brief Allocate shared memory.
     * @param key Shared memory key.
     * @param size Size of the shared memory.
     * @return True if allocation is successful, false otherwise.
     */
    bool allocate(int key, int size);

    /**
     * @brief Get the number of attached processes.
     * @return The number of attached processes.
     */
    int get_attached_number();
};