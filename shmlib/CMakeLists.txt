add_library(SHLib STATIC
    src/notifier.cpp
    src/Buffer.cpp
)

target_sources(SHLib PRIVATE src/SharedMemory.cpp)

target_link_libraries(SHLib PRIVATE pthread)

target_include_directories(SHLib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)