#include <atomic>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "sharedMemory.h"
#include "notifier.h"
#include "buffer.h"
#include "utility.h"
#include <signal.h>
#include <thread>
#include <chrono>

using namespace std;

atomic<bool> terminate_flag(false);

void signalHandler(int signum) {
    terminate_flag = true;
}

void makeDataArray(std::vector<uint8_t> &dataArray, const std::vector<uint8_t> &receivedArray)
{
    // Define the fixed parts of the data array
    std::vector<uint8_t> id = {0, 0, 2};
    std::vector<uint8_t> d_id = {0, 0, 1};
    // Extract the trace, mti, and pan from the received array
    std::vector<uint8_t> trace(receivedArray.begin() + 7, receivedArray.begin() + 7 + 6);
    std::vector<uint8_t> mti(receivedArray.begin() + 3, receivedArray.begin() + 3 + 4);
    std::vector<uint8_t> pan(receivedArray.begin() + 13, receivedArray.begin() + 13 + 16);
    // Modify the mti by adding 10
    addToVector<int>(mti, 10);
    // Clear the data array to ensure it's empty before appending
    dataArray.clear();
    // Append the fixed parts and the extracted parts to the data array
    appendArrays(dataArray, id);
    appendArrays(dataArray, mti);
    appendArrays(dataArray, trace);
    appendArrays(dataArray, pan);
    appendArrays(dataArray, d_id);
}

int main()
{
    signal(SIGTERM, signalHandler);
    
    std::vector<uint8_t> targetArray = {11, 12, 13};

    Notifer buffLck;
    buffLck.init(0xfa00);

    Buffer buffer;
    buffer.init(0x3333);

    std::vector<uint8_t> data = {};

    while (!terminate_flag)
    {
        if (buffLck.wait())
        {
            std::vector<uint8_t> valueBack;
            valueBack.resize(32);
            valueBack = buffer.read();
            std::cout << "2: received value is: " << std::endl;
            for (int i = 0; i < 32; i++)
            {
                cout << static_cast<int>(valueBack[i]);
            }
            cout << std::endl;
            // a pause to let sender get into wainitng mode.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            makeDataArray(data,valueBack);
            buffer.write(data);
            std::cout << "2: sent value is: " << std::endl;
            for (int i = 0; i < 32; i++)
            {
                cout << static_cast<int>(data[i]);
            }
            cout << std::endl;
            buffLck.notify();
        }
    }
}
