cmake_minimum_required(VERSION 3.10)

project(shMem)

add_subdirectory(shmlib)

add_executable(Sender   main/sender.cpp)
add_executable(Receiver   main/receiver.cpp)
add_executable(EntryPoint main/entryPoint.cpp)

target_link_libraries(Sender SHLib)
target_link_libraries(Receiver SHLib)

add_dependencies(EntryPoint Sender Receiver)