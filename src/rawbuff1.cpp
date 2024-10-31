#include <stdio.h>
#include <stdint.h>
#include "SharedMemory.h"
#include "Buffer.h"


int main()
{

    RawBuffer3 rb_, *rb;
    uint8_t *bufdata;
    uint8_t *bufwrtn;

    SharedMemory buf_data;
    bool inited = buf_data.init((void**)&bufdata, 0x12345, 1000);

    SharedMemory buf_wrtn;
    buf_data.init((void**)&bufwrtn, 0x12346, 8);

    rbInit3RawBuffer3(&rb_, 1000, 1, bufdata, (int64_t*)bufwrtn);
    rb = &rb_;
    if (inited) rbReset(rb);

    uint8_t temp_buf[1000];
    for(int i=0; i<40; i++)
        temp_buf[i] = i;
    rbPush(rb, 1, temp_buf, 40);
    while(true){
        
    }
}