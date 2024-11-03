# Shared Memory IPC Project

This project demonstrates inter-process communication (IPC) between a sender and receiver process using shared memory in C++. It includes a shared memory library, supervisor application, utility functions, and tests to ensure robust functionality.

## Components

### Shared Memory Library (`shmlib`)
- **Buffer**: Manages a shared memory buffer for data read/write operations.
- **Notifier**: Handles synchronization between processes using condition variables and mutexes.
- **SharedMemory**: Provides an interface for allocating, attaching, and releasing shared memory.

### Supervisor Application (`supervisor`)
- **EntryPoint**: Main entry point that forks and manages sender and receiver processes. It also handles termination signals to ensure both processes are properly terminated.
- **Sender**: Reads user input, constructs a data array, writes it to the shared memory buffer, and waits for a response from the receiver.
- **Receiver**: Waits for a notification from the sender, reads data from the shared memory buffer, processes it, writes the result back to the buffer, and notifies the sender.

### Tests (`tests`)
- **BufferTest**: Verifies initialization and data read/write operations for the `Buffer` class.
- **NotifierTest**: Ensures proper functionality of the `Notifier` class, including initialization and notification/wait operations.

### Utility Functions (`utility.h`)
- Contains helper functions for appending arrays, getting user input, and managing vector data.

### CMake Configuration
- This project uses CMake to manage dependencies, build the library, executables, and tests. It also includes a configuration for generating Doxygen documentation.

## Getting Started

### Prerequisites
- **CMake**: Download from [cmake.org](https://cmake.org/download/).
- **C++ Compiler**: Ensure you have GCC, Clang, or another compatible C++ compiler.

### Clone the Repository
```sh
git clone https://github.com/morteza730/small_IPC_shared_memory.git
cd small_IPC_shared_memory
```
### Build the Project
1. Create a build directory:
```sh
mkdir build
cd build
```
2. Run CMake to generate the build files:
```sh
cmake ..
```
2. Build the project:
```sh
make
```

### Run the Tests
After building the project, you can run the tests:
```sh
make test
```

### Run the Tests
To generate Doxygen documentation, run:
```sh
make doc
```
The documentation will be generated in the build/doc/html directory.

### Run the Supervisor Application
To run the supervisor application, execute:
```sh
./supervisor/EntryPoint
```
