#include <atomic>
#include <iostream>
#include <csignal>
#include <unistd.h>
#include "SharedMemory.h"
#include "subprocess.h"
#include "notifier.h"
#include "Buffer.h"

using namespace std;

atomic<bool> terminate_flag(false);
// void signalHandler(int signum) {terminate_flag = true;}

int main()
{
    Notifer buffLck;
    buffLck.init(0xfa00);

    Buffer buffer;
    buffer.init(0x3333);

    uint8_t value;

    while (!terminate_flag)
    {
        if (buffLck.wait())
        {
            printf("a = %hhu \n", buffer.read());
            buffer.write(35);
            buffLck.notify();
        }
    }
}

// int main()
// {
//     uint8_t id_num[3] = {0,0,2};
//     uint8_t mti_num[4] = {1,2,3,4};
//     uint8_t pan_num[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
//     uint8_t destination_num[3] = {0,0,1};

//     Notifer buffLck;
//     buffLck.init(0xfa00);

//     Message* msg = new Message{};
//     SharedMemory sMemory;
//     if (sMemory.init((void **)&msg, 0x1234, 40))
//     {
//         if (msg == nullptr)
//             return -1;
//         printf("You are the first one! Please init the memory if needed!\n");
//     }

//     while (!terminate_flag)
//     {
//         if (buffLck.wait())
//         {
//             printf("a = %i\n", msg->id[2]);
//             msg->id[2] = 2;
//         }
//         // sProcess.sendback();
//         buffLck.notify();
//     }
// }