# Minimal Shared Memory IPC Project

This project is a C++ application that demonstrates the use of shared memory for inter-process communication (IPC) between a sender and a receiver process. The project consists of several components:

## Shared Memory Library (`shmlib`)
- **Buffer**: Manages a shared memory buffer for reading and writing data.
- **Notifier**: Handles synchronization between processes using condition variables and mutexes.
- **SharedMemory**: Provides an interface for allocating, attaching to, and releasing shared memory.

## Supervisor Application (`supervisor`)
- **EntryPoint**: The main entry point that forks and executes the sender and receiver processes. It also handles termination signals to ensure both processes are properly terminated.
- **Sender**: Reads user input, constructs a data array, writes it to the shared memory buffer, and waits for a notification from the receiver.
- **Receiver**: Waits for a notification from the sender, reads the data from the shared memory buffer, processes it, writes the processed data back to the buffer, and notifies the sender.

## Tests (`tests`)
- **BufferTest**: Unit tests for the `Buffer` class to ensure proper initialization and data read/write operations.
- **NotifierTest**: Unit tests for the `Notifier` class to ensure proper initialization and notification/wait operations.

## Utility Functions (`utility.h`)
- Provides helper functions for appending arrays, getting user input, and adding values to vectors.

## CMake Configuration
- The project uses CMake for building and managing dependencies. It includes configurations for building the shared library, executables, and tests, as well as generating Doxygen documentation.

## Getting Started
To get started with this project, follow these steps:

1. **Clone the repository**:
   ```sh
   git clone https://github.com/morteza730/small_IPC_shared_memory.git
   cd small_IPC_shared_memory
