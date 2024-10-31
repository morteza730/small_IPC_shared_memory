#include <atomic>
#include <csignal>
#include "SharedMemory.h"
#include "notifier.h"
#include "subprocess.h"
#include "Buffer.h"
#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;

int main()
{
    Notifer buffLck;
    buffLck.init(0xfa00);

    Buffer buffer;
    buffer.init(0x3333);

    uint8_t value;

    std::cout << "please enter a value>> " << endl;
    do
    {
        cin >> value;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (cin)
        {
            std::cout<<"sent value is: "<<value<<std::endl;
            printf("sent value is: %hhu\n", value);
            buffer.write(value);
            buffLck.notify();
        }
        if (buffLck.wait())
        {
            printf("received value is: %hhu\n", buffer.read());
        }
    } while (cin);
}

// int main()
// {
//     uint8_t id_num[3] = {0,0,1};
//     uint8_t mti_num[4] = {1,2,3,4};
//     uint8_t pan_num[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
//     uint8_t destination_num[3] = {0,0,2};

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

//     // SubProcess sProcess{id_num,mti_num,pan_num};

//     std::cout << "please enter the trace value>> " << endl;
//     uint8_t trace[6];
//     do
//     {
//         cin >> trace;
//         cin.clear(); // Clear the error state
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         if (cin)
//         {
//             for (int i=0;i<6;i++)
//                 msg->Trace[i] = trace[i];
//             buffLck.notify();
//         }
//         if (buffLck.wait())
//         {
//             printf("a = %i\n", msg->id[2]);
//         }
//     } while (cin);
// }