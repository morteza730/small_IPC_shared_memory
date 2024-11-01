#include <atomic>
#include <csignal>
#include "SharedMemory.h"
#include "notifier.h"
#include "Buffer.h"
#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;

int main()
{
    int id = 1;
    
    Notifer buffLck;
    buffLck.init(0xfa00);

    Buffer buffer;
    buffer.init(0x3333);

    uint8_t value[6];

    std::cout << "please enter a value>> " << endl;
    do
    {
        cin >> value;
        if (cin)
        {
            std::cout << id << ":sent value is: " << value[0] << std::endl;
            buffer.write(value[0]);
            buffLck.notify();
        }
        else
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (buffLck.wait())
        {
            std::cout << id << ": received value is: " << buffer.read() << std::endl;
        }
    } while (cin);
}