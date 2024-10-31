#include <atomic>
#include <csignal>
#include "SharedMemory.h"
#include "InterProccessNotifier.h"
#include "subprocess.h"
#include "Buffer.h"
#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;

int main()
{
    uint8_t id_num[3] = {0,0,1};
    uint8_t mti_num[4] = {1,2,3,4};
    uint8_t pan_num[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    uint8_t destination_num[3] = {0,0,2};

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

    // SubProcess sProcess{id_num,mti_num,pan_num};

    std::cout << "please enter the trace value>> " << endl;
    uint8_t trace[6];
    do
    {
        cin >> trace;
        cin.clear(); // Clear the error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (cin)
        {
            for (int i=0;i<6;i++)
                msg->Trace[i] = trace[i];
            buffLck.notify();
        }
        if (buffLck.wait() != 0)
        {
            printf("a = %i\n", msg->id[2]);
        }
    } while (cin);
}

// int main()
// {
//     Buffer *buffer = nullptr;
//     SharedMemory mcb;

//     InterProccNotifier buffLck;
//     buffLck.init(0xfa00);

//     if (mcb.init((void **)&buffer, 0x1234, 40))
//     {
//         if (buffer == nullptr)
//             return -1;
//         printf("You are the first one! Please init the memory if needed!\n");
//     }
//     buffer->id = 0x001;
//     buffer->MTI = 0x2210;
//     buffer->Trace = 0x123456;
//     uint8_t a[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
//     buffer->PAN = a;
//     char choice;

//     std::cout << "Enter \"s\" (to notify) and \"q\" (for quit) >> " << endl;
//     do
//     {
//         cin >> choice;
//         cin.clear(); // Clear the error state
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         if (choice == 's')
//         {
//             buffLck.notify();
//             std::cout << "Notification!" << std::endl;
//         }
//         if (buffLck.wait() != 0)
//         {
//             printf("a = %i\n", buffer->id);
//         }
//     } while (cin);
// }