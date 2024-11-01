#include <atomic>
#include <iostream>
#include <csignal>
#include <unistd.h>
#include "SharedMemory.h"
#include "notifier.h"
#include "Buffer.h"

using namespace std;

atomic<bool> terminate_flag(false);

int main()
{
    int id = 2;

    Notifer buffLck;
    buffLck.init(0xfa00);

    Buffer buffer;
    buffer.init(0x3333);

    while (!terminate_flag)
    {
        if (buffLck.wait())
        {
            char valueBack = buffer.read();
            std::cout << id << ": received value is: " << valueBack << std::endl;
            buffer.write('b');
            std::cout << id << ": sent value is: " << 'b' << std::endl;
            buffLck.notify();
        }
    }
}
