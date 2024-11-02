#include <atomic>
#include "sharedMemory.h"
#include "notifier.h"
#include "buffer.h"
#include <iostream>
#include <unistd.h>
#include <vector>
#include "utility.h"
#include <signal.h>

using namespace std;

atomic<bool> terminate_flag(false);

void signalHandler(int signum) {
    terminate_flag = true;
}

void makeDataArray(std::vector<uint8_t> &dataArray, const std::vector<uint8_t> &trace)
{
    std::vector<uint8_t> id = {0, 0, 1};
    std::vector<uint8_t> mti = {2, 2, 0, 0};
    std::vector<uint8_t> pan = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::vector<uint8_t> d_id = {0, 0, 2};

    dataArray.clear();

    appendArrays(dataArray, id);
    appendArrays(dataArray, mti);
    appendArrays(dataArray, trace);
    appendArrays(dataArray, pan);
    appendArrays(dataArray, d_id);
}

int main()
{
    signal(SIGTERM, signalHandler);

    std::vector<uint8_t> data = {};

    Notifer buffLck;
    buffLck.init(0xfa00);

    Buffer buffer;
    buffer.init(0x3333);

    uint8_t value[6];

    while (!terminate_flag)
    {
        std::vector<uint8_t> userArray = getUserInput<uint8_t>();
        if (userArray.size() < 6)
        {
            std::cerr << "trace size is shorter than 6: " << userArray.size() << std::endl;
            continue;
        }
        makeDataArray(data, userArray);
        buffer.write(data);
        std::cout << "1: sent value is: " << std::endl;
            for (int i = 0; i < 32; i++)
            {
                cout << static_cast<int>(data[i]);
            }
        std::cout << std::endl;
        buffLck.notify();
        if (buffLck.wait())
        {
            std::vector<uint8_t> valueBack;
            valueBack.resize(32);
            valueBack = buffer.read();
            std::cout << "1: received value is: " << std::endl;
            for (int i=0;i<32;i++)
            {
                cout << static_cast<int>(valueBack[i]);
            }
            cout << std::endl;
        }
    }
}