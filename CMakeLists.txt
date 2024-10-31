cmake_minimum_required(VERSION 3.10)

project(shMem)

include_directories(..)
set(SHM_SRC_FILES src/SharedMemory.cpp src/InterProccessNotifier.cpp src/Subprocess.cpp)

add_executable(m1   src/m1.cpp ${SHM_SRC_FILES})
add_executable(m2   src/m2.cpp ${SHM_SRC_FILES})
add_executable(EntryPoint src/entryPoint.cpp)

target_link_libraries(m1 pthread)
target_link_libraries(m2 pthread)

add_dependencies(EntryPoint m1 m2)

# add_executable(rawbuff1   rawbuff1.cpp ${SHM_SRC_FILES} nsrRawBuffer3.c)
# add_executable(rawbuff2   rawbuff2.cpp ${SHM_SRC_FILES} nsrRawBuffer3.c)