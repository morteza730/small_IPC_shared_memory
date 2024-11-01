#include <atomic>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "sharedMemory.h"
#include "notifier.h"
#include "buffer.h"
#include "utility.h"

using namespace std;

atomic<bool> terminate_flag(false);

void makeDataArray(std::vector<uint8_t> &dataArray)
{
    std::vector<uint8_t> id = {0, 0, 2};
    std::vector<uint8_t> mti = {2, 2, 1, 0};
    std::vector<uint8_t> trace = {1, 2, 3, 4, 5, 6};
    std::vector<uint8_t> pan = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::vector<uint8_t> d_id = {0, 0, 1};

    appendArrays(dataArray, id);
    appendArrays(dataArray, mti);
    appendArrays(dataArray, trace);
    appendArrays(dataArray, pan);
    appendArrays(dataArray, d_id);
}

int main()
{
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
            makeDataArray(data);
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
