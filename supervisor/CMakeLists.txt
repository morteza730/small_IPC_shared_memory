add_executable(Sender   src/sender.cpp)
target_link_libraries(Sender SHLib)
target_include_directories(Sender PRIVATE include)

add_executable(Receiver   src/receiver.cpp)
target_link_libraries(Receiver SHLib)
target_include_directories(Receiver PRIVATE include)

add_executable(EntryPoint src/entryPoint.cpp)
add_dependencies(EntryPoint Sender Receiver)