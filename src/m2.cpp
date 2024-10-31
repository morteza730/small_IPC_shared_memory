#include <atomic>
#include <iostream>
#include <csignal>
#include <unistd.h>
#include "SharedMemory.h"
#include "subprocess.h"
#include "InterProccessNotifier.h"
#include "Buffer.h"

using namespace std;

atomic<bool> terminate_flag(false);
// void signalHandler(int signum) {terminate_flag = true;}

int main()
{
    uint8_t id_num[3] = {0,0,2};
    uint8_t mti_num[4] = {1,2,3,4};
    uint8_t pan_num[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    uint8_t destination_num[3] = {0,0,1};

    InterProccNotifier buffLck;
    buffLck.init(0xfa00);

    Message* msg = new Message{};
    SharedMemory sMemory;
    if (sMemory.init((void **)&msg, 0x1234, 40))
    {
        if (msg == nullptr)
            return -1;
        printf("You are the first one! Please init the memory if needed!\n");
    }

    while (!terminate_flag)
    {
        if (buffLck.wait() != 0)
        {
            printf("a = %i\n", msg->id[2]);
            msg->id[2] = 2;
        }
        // sProcess.sendback();
        buffLck.notify();
    }
}

// int main()
// {
//     Buffer *buffer;
//     SharedMemory mcb;
//     InterProccNotifier buffLck;
//     buffLck.init(0xfa00);
//     if (mcb.init((void **)&buffer, 0x1234, 40))
//     {
//         if (buffer == nullptr)
//             return -1;
//         printf("You are the first one! Please init the memory if needed!\n");
//     }

//     while (!terminate_flag)
//     {
//         if (buffLck.wait() != 0)
//         {
//             printf("a = %i\n", buffer->id);
//             buffer->id = 0x002;
//             buffLck.notify();
//         }
//     }
// }